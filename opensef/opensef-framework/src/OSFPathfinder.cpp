/**
 * OSFPathfinder.cpp - Universal Command Center Implementation
 *
 * ONE interface for EVERYTHING users need to find and do.
 * Integrates with: apps, files, clipboard, NixOS, web search.
 */

#include "opensef/OSFPathfinder.h"
#include "opensef/OSFClipboard.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>

#define LOG(msg) std::cout << "[Pathfinder] " << msg << std::endl

namespace OpenSEF {

struct Pathfinder::Implementation {
  bool visible = false;
  std::string currentQuery;
  ResultsCallback callback = nullptr;

  // Execute shell command and get output
  std::string exec(const char *cmd) {
    char buffer[4096];
    std::string result;
    FILE *pipe = popen(cmd, "r");
    if (!pipe)
      return "";

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
      result += buffer;
    }
    pclose(pipe);
    return result;
  }

  // Parse nix search output
  std::vector<SearchResult> parseNixSearchOutput(const std::string &output) {
    std::vector<SearchResult> results;
    std::istringstream stream(output);
    std::string line;

    while (std::getline(stream, line)) {
      // nix search format: "* nixpkgs.packagename (version)"
      if (line.find("* nixpkgs.") != std::string::npos) {
        SearchResult result;
        result.type = SearchResultType::NixPackage;

        // Extract package name
        size_t start = line.find("nixpkgs.") + 8;
        size_t end = line.find(" ", start);
        if (end == std::string::npos)
          end = line.length();

        result.id = line.substr(start, end - start);
        result.title = result.id;
        result.subtitle = "Press Enter to install via NixOS";
        result.icon = "package";
        result.priority = 50;

        results.push_back(result);
        if (results.size() >= 5)
          break; // Limit results
      }
    }
    return results;
  }
};

Pathfinder &Pathfinder::shared() {
  static Pathfinder instance;
  return instance;
}

Pathfinder::Pathfinder() : impl_(std::make_unique<Implementation>()) {
  LOG("Initialized - Universal Command Center");
}

Pathfinder::~Pathfinder() = default;

void Pathfinder::show() {
  LOG("Show");
  impl_->visible = true;
  // AnimationEngine will handle fade-in
}

void Pathfinder::hide() {
  LOG("Hide");
  impl_->visible = false;
}

void Pathfinder::toggle() {
  if (impl_->visible) {
    hide();
  } else {
    show();
  }
}

bool Pathfinder::isVisible() const { return impl_->visible; }

SearchResults Pathfinder::search(const std::string &query) {
  impl_->currentQuery = query;

  SearchResults results;

  if (query.empty()) {
    return results;
  }

  // Search all sources simultaneously
  results.apps = searchApplications(query);
  results.files = searchFiles(query);
  results.clipboard = searchClipboard(query);
  results.packages = searchNixPackages(query);
  results.webActions = prepareWebSearch(query);
  results.systemActions = searchSystemActions(query);

  if (impl_->callback) {
    impl_->callback(results);
  }

  return results;
}

void Pathfinder::setQuery(const std::string &query) { search(query); }

std::vector<SearchResult>
Pathfinder::searchApplications(const std::string &query) {
  std::vector<SearchResult> results;

  // Search .desktop files in standard locations
  std::string cmd = "find /usr/share/applications ~/.local/share/applications "
                    "-name '*.desktop' 2>/dev/null | xargs -I{} grep -l -i '" +
                    query + "' {} 2>/dev/null | head -5";

  std::string output = impl_->exec(cmd.c_str());
  std::istringstream stream(output);
  std::string line;

  while (std::getline(stream, line)) {
    if (!line.empty()) {
      SearchResult result;
      result.type = SearchResultType::Application;
      result.id = line;

      // Extract app name from path
      size_t lastSlash = line.rfind('/');
      size_t lastDot = line.rfind('.');
      if (lastSlash != std::string::npos && lastDot != std::string::npos) {
        result.title = line.substr(lastSlash + 1, lastDot - lastSlash - 1);
      } else {
        result.title = line;
      }

      result.subtitle = "Application";
      result.icon = "application";
      result.priority = 100; // Apps first
      results.push_back(result);
    }
  }

  return results;
}

std::vector<SearchResult> Pathfinder::searchFiles(const std::string &query) {
  std::vector<SearchResult> results;

  // Use locate or find for file search
  std::string cmd = "locate -i -l 5 '*" + query +
                    "*' 2>/dev/null || "
                    "find ~ -iname '*" +
                    query + "*' 2>/dev/null | head -5";

  std::string output = impl_->exec(cmd.c_str());
  std::istringstream stream(output);
  std::string line;

  while (std::getline(stream, line)) {
    if (!line.empty()) {
      SearchResult result;
      result.type = SearchResultType::File;
      result.id = line;

      size_t lastSlash = line.rfind('/');
      result.title =
          (lastSlash != std::string::npos) ? line.substr(lastSlash + 1) : line;
      result.subtitle = line;
      result.icon = "file";
      result.priority = 70;
      results.push_back(result);
    }
  }

  return results;
}

std::vector<SearchResult>
Pathfinder::searchClipboard(const std::string &query) {
  std::vector<SearchResult> results;

  // Search clipboard history from OSFClipboard
  auto history = OSFClipboard::shared().history();
  std::string lowerQuery = query;
  std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(),
                 ::tolower);

  int index = 0;
  for (const auto &item : history) {
    if (item.type == OSFClipboardType::Text && !item.text.empty()) {
      std::string lowerText = item.text;
      std::transform(lowerText.begin(), lowerText.end(), lowerText.begin(),
                     ::tolower);

      if (lowerText.find(lowerQuery) != std::string::npos) {
        SearchResult result;
        result.type = SearchResultType::ClipboardItem;
        result.id = "clipboard:" + std::to_string(index);

        // Truncate for display
        if (item.text.length() > 50) {
          result.title = item.text.substr(0, 47) + "...";
        } else {
          result.title = item.text;
        }

        result.subtitle = "Clipboard history";
        result.icon = "clipboard";
        result.priority = 60;

        results.push_back(result);
        if (results.size() >= 5)
          break; // Limit results
      }
    }
    index++;
  }

  return results;
}

std::vector<SearchResult>
Pathfinder::searchNixPackages(const std::string &query) {
  std::vector<SearchResult> results;

  if (query.length() < 2) {
    return results; // Don't search for very short queries
  }

  // Query NixOS packages
  std::string cmd = "nix search nixpkgs " + query + " 2>/dev/null | head -20";
  std::string output = impl_->exec(cmd.c_str());

  results = impl_->parseNixSearchOutput(output);

  return results;
}

std::vector<SearchResult>
Pathfinder::prepareWebSearch(const std::string &query) {
  std::vector<SearchResult> results;

  SearchResult zenSearch;
  zenSearch.type = SearchResultType::WebSearch;
  zenSearch.id = "web:" + query;
  zenSearch.title = "Search \"" + query + "\" in browser";
  zenSearch.subtitle = "Open web search";
  zenSearch.icon = "web";
  zenSearch.priority = 10; // Low priority - fallback

  results.push_back(zenSearch);

  return results;
}

std::vector<SearchResult>
Pathfinder::searchSystemActions(const std::string &query) {
  std::vector<SearchResult> results;
  std::string lowerQuery = query;
  std::transform(lowerQuery.begin(), lowerQuery.end(), lowerQuery.begin(),
                 ::tolower);

  // System actions
  if (lowerQuery.find("shut") != std::string::npos ||
      lowerQuery.find("power") != std::string::npos) {
    SearchResult result;
    result.type = SearchResultType::SystemAction;
    result.id = "action:shutdown";
    result.title = "Shutdown";
    result.subtitle = "Power off this computer";
    result.icon = "power";
    result.priority = 80;
    results.push_back(result);
  }

  if (lowerQuery.find("rest") != std::string::npos ||
      lowerQuery.find("reboot") != std::string::npos) {
    SearchResult result;
    result.type = SearchResultType::SystemAction;
    result.id = "action:restart";
    result.title = "Restart";
    result.subtitle = "Restart this computer";
    result.icon = "restart";
    result.priority = 80;
    results.push_back(result);
  }

  if (lowerQuery.find("lock") != std::string::npos) {
    SearchResult result;
    result.type = SearchResultType::SystemAction;
    result.id = "action:lock";
    result.title = "Lock Screen";
    result.subtitle = "Lock the display";
    result.icon = "lock";
    result.priority = 80;
    results.push_back(result);
  }

  if (lowerQuery.find("settings") != std::string::npos ||
      lowerQuery.find("pref") != std::string::npos) {
    SearchResult result;
    result.type = SearchResultType::SystemAction;
    result.id = "action:settings";
    result.title = "System Settings";
    result.subtitle = "Open system preferences";
    result.icon = "settings";
    result.priority = 80;
    results.push_back(result);
  }

  return results;
}

void Pathfinder::execute(const SearchResult &result) {
  LOG("Execute: " << result.title);

  switch (result.type) {
  case SearchResultType::Application:
    // Launch via xdg-open or gtk-launch
    {
      std::string cmd = "gtk-launch " + result.id + " &";
      system(cmd.c_str());
    }
    break;

  case SearchResultType::File:
    // Open with xdg-open
    {
      std::string cmd = "xdg-open '" + result.id + "' &";
      system(cmd.c_str());
    }
    break;

  case SearchResultType::NixPackage:
    installNixPackage(result.id);
    break;

  case SearchResultType::WebSearch:
    // Open default browser
    {
      std::string query = result.id.substr(4); // Remove "web:" prefix
      std::string cmd =
          "xdg-open 'https://www.google.com/search?q=" + query + "' &";
      system(cmd.c_str());
    }
    break;

  case SearchResultType::SystemAction:
    if (result.id == "action:shutdown") {
      system("systemctl poweroff");
    } else if (result.id == "action:restart") {
      system("systemctl reboot");
    } else if (result.id == "action:lock") {
      // TODO: Lock screen via openSEF
    } else if (result.id == "action:settings") {
      system("osf-settings &");
    }
    break;

  default:
    break;
  }

  hide();
}

void Pathfinder::installNixPackage(const std::string &packageName) {
  LOG("Installing NixOS package: " << packageName);

  // TODO: Show progress via AnimationEngine
  std::string cmd = "nix-env -iA nixpkgs." + packageName;
  system(cmd.c_str());

  LOG("Package installed: " << packageName);
}

void Pathfinder::onResultsChanged(ResultsCallback callback) {
  impl_->callback = callback;
}

std::vector<SearchResult> SearchResults::sorted() const {
  std::vector<SearchResult> all;

  // Combine all results
  all.insert(all.end(), apps.begin(), apps.end());
  all.insert(all.end(), files.begin(), files.end());
  all.insert(all.end(), clipboard.begin(), clipboard.end());
  all.insert(all.end(), packages.begin(), packages.end());
  all.insert(all.end(), systemActions.begin(), systemActions.end());
  all.insert(all.end(), webActions.begin(), webActions.end());

  // Sort by priority (descending)
  std::sort(all.begin(), all.end(),
            [](const SearchResult &a, const SearchResult &b) {
              return a.priority > b.priority;
            });

  return all;
}

} // namespace OpenSEF

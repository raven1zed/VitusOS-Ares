#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace OpenSEF {

/**
 * OSFPathfinder - Universal Command Center
 *
 * ONE interface for EVERYTHING users need to find and do:
 *  - Application search & launch
 *  - File search (integrated with Filer index)
 *  - Clipboard history search
 *  - NixOS package installation (no separate app store)
 *  - Web search fallback
 *  - System actions
 *
 * Lives in Filer (always-running), shares its indexes.
 * Activated via Cmd+Space (global shortcut).
 */

enum class SearchResultType {
  Application,
  File,
  ClipboardItem,
  NixPackage,
  WebSearch,
  SystemAction
};

struct SearchResult {
  SearchResultType type;
  std::string id;
  std::string title;
  std::string subtitle;
  std::string icon;
  int priority; // Higher = more relevant
};

struct SearchResults {
  std::vector<SearchResult> apps;
  std::vector<SearchResult> files;
  std::vector<SearchResult> clipboard;
  std::vector<SearchResult> packages;
  std::vector<SearchResult> webActions;
  std::vector<SearchResult> systemActions;

  std::vector<SearchResult> sorted() const;
};

class Pathfinder {
public:
  static Pathfinder &shared();

  // UI Control
  void show();
  void hide();
  void toggle();
  bool isVisible() const;

  // Search
  SearchResults search(const std::string &query);
  void setQuery(const std::string &query);

  // Execute result
  void execute(const SearchResult &result);

  // Individual search functions
  std::vector<SearchResult> searchApplications(const std::string &query);
  std::vector<SearchResult> searchFiles(const std::string &query);
  std::vector<SearchResult> searchClipboard(const std::string &query);
  std::vector<SearchResult> searchNixPackages(const std::string &query);
  std::vector<SearchResult> prepareWebSearch(const std::string &query);
  std::vector<SearchResult> searchSystemActions(const std::string &query);

  // Package installation
  void installNixPackage(const std::string &packageName);

  // Callbacks
  using ResultsCallback = std::function<void(const SearchResults &)>;
  void onResultsChanged(ResultsCallback callback);

  Pathfinder(const Pathfinder &) = delete;
  Pathfinder &operator=(const Pathfinder &) = delete;
  ~Pathfinder();

private:
  friend class Filer; // Pathfinder lives in Filer
  Pathfinder();

  struct Implementation;
  std::unique_ptr<Implementation> impl_;
};

} // namespace OpenSEF

#!/usr/bin/env bash
set -euo pipefail

range="${1:-}"

if [[ -z "$range" ]]; then
  if [[ -n "${GITHUB_BASE_REF:-}" ]]; then
    git fetch origin "${GITHUB_BASE_REF}" --depth=1
    range="origin/${GITHUB_BASE_REF}..HEAD"
  else
    range="HEAD~1..HEAD"
  fi
fi

pattern='^(feat|fix|docs|refactor|test|chore): .+'
invalid=0

while read -r sha; do
  subject=$(git log --format=%s -n 1 "$sha")
  if [[ ! "$subject" =~ $pattern ]]; then
    echo "Invalid commit message: $sha" >&2
    echo "  $subject" >&2
    invalid=1
  fi
done < <(git rev-list --no-merges "$range")

if [[ "$invalid" -ne 0 ]]; then
  echo "Commit message format must match: type: brief description" >&2
  exit 1
fi

/**
 * OSFTilingManager.cpp - Tiling window manager
 */

#include <opensef/OSFWindowDecorations.h>

namespace opensef {

OSFTilingManager &OSFTilingManager::shared() {
  static OSFTilingManager instance;
  return instance;
}

OSFTilingZone OSFTilingManager::zoneAtPosition(float x, float y,
                                               float screenWidth,
                                               float screenHeight) {
  bool atLeft = x < kEdgeThreshold;
  bool atRight = x > screenWidth - kEdgeThreshold;
  bool atTop = y < kEdgeThreshold;
  bool atBottom = y > screenHeight - kEdgeThreshold;

  // Corners (quarters)
  if (atTop && atLeft)
    return OSFTilingZone::TopLeft;
  if (atTop && atRight)
    return OSFTilingZone::TopRight;
  if (atBottom && atLeft)
    return OSFTilingZone::BottomLeft;
  if (atBottom && atRight)
    return OSFTilingZone::BottomRight;

  // Edges (halves)
  if (atLeft)
    return OSFTilingZone::Left;
  if (atRight)
    return OSFTilingZone::Right;

  // Top edge = maximize
  if (atTop)
    return OSFTilingZone::Maximize;

  return OSFTilingZone::None;
}

OSFRect OSFTilingManager::frameForZone(OSFTilingZone zone, float screenWidth,
                                       float screenHeight) {
  float halfWidth = screenWidth / 2;
  float halfHeight = screenHeight / 2;

  switch (zone) {
  case OSFTilingZone::Left:
    return OSFRect(0, 0, halfWidth, screenHeight);

  case OSFTilingZone::Right:
    return OSFRect(halfWidth, 0, halfWidth, screenHeight);

  case OSFTilingZone::TopLeft:
    return OSFRect(0, 0, halfWidth, halfHeight);

  case OSFTilingZone::TopRight:
    return OSFRect(halfWidth, 0, halfWidth, halfHeight);

  case OSFTilingZone::BottomLeft:
    return OSFRect(0, halfHeight, halfWidth, halfHeight);

  case OSFTilingZone::BottomRight:
    return OSFRect(halfWidth, halfHeight, halfWidth, halfHeight);

  case OSFTilingZone::Maximize:
    return OSFRect(0, 0, screenWidth, screenHeight);

  case OSFTilingZone::None:
  default:
    return OSFRect::Zero();
  }
}

} // namespace opensef

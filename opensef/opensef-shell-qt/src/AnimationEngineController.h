#pragma once

#include <QObject>
#include <QTimer>
#include <opensef/OSFAnimationEngine.h>
#include <opensef/OSFDesktop.h>

/**
 * AnimationEngineController - QML Bridge for AnimationEngine
 *
 * Exposes C++ AnimationEngine API to QML for boot sequence and animations.
 */
class AnimationEngineController : public QObject {
  Q_OBJECT

  Q_PROPERTY(float bootProgress READ bootProgress WRITE setBootProgress NOTIFY
                 bootProgressChanged)
  Q_PROPERTY(bool bootSequenceActive READ isBootSequenceActive NOTIFY
                 bootSequenceActiveChanged)

public:
  explicit AnimationEngineController(QObject *parent = nullptr);

  float bootProgress() const;
  void setBootProgress(float progress);
  bool isBootSequenceActive() const;

  Q_INVOKABLE void startBootSequence();
  Q_INVOKABLE void transitionToDesktop();

signals:
  void bootProgressChanged();
  void bootSequenceActiveChanged();

private slots:
  void updateProgress();

private:
  float progress_ = 0.0f;
  bool bootActive_ = false;
  QTimer *progressTimer_ = nullptr;
};

#include "AnimationEngineController.h"
#include <QDebug>

AnimationEngineController::AnimationEngineController(QObject *parent)
    : QObject(parent), progress_(0.0f), bootActive_(false) {
  progressTimer_ = new QTimer(this);
  connect(progressTimer_, &QTimer::timeout, this,
          &AnimationEngineController::updateProgress);
}

float AnimationEngineController::bootProgress() const { return progress_; }

void AnimationEngineController::setBootProgress(float progress) {
  if (progress_ != progress) {
    progress_ = progress;
    emit bootProgressChanged();
  }
}

bool AnimationEngineController::isBootSequenceActive() const {
  return bootActive_;
}

void AnimationEngineController::startBootSequence() {
  qDebug() << "[AnimationEngineController] Starting boot sequence";

  auto *engine = OpenSEF::OSFDesktop::shared()->animationEngine();
  engine->playBootSequence();

  bootActive_ = true;
  emit bootSequenceActiveChanged();

  // Start polling systemd progress
  progressTimer_->start(200); // 200ms polling
}

void AnimationEngineController::transitionToDesktop() {
  qDebug() << "[AnimationEngineController] Transition to desktop";

  progressTimer_->stop();
  bootActive_ = false;

  auto *engine = OpenSEF::OSFDesktop::shared()->animationEngine();
  engine->transitionToDesktop();

  emit bootSequenceActiveChanged();
}

void AnimationEngineController::updateProgress() {
  // In WSL, systemd targets are already reached, so simulate boot progress
  // In real boot, this would use: engine->querySystemdProgress()

  // Simulated progress for WSL testing - increment by 0.1 every tick
  float newProgress = progress_ + 0.1f;
  if (newProgress > 1.0f) {
    newProgress = 1.0f;
  }

  qDebug() << "[AnimationEngineController] Boot progress:" << newProgress * 100
           << "%";
  setBootProgress(newProgress);

  // Auto-complete when at 100%
  if (newProgress >= 1.0f) {
    transitionToDesktop();
  }
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QListWidget>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QStringList>

QT_BEGIN_NAMESPACE
class QListWidgetItem;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddFiles();
    void onPlayPause();
    void onStop();
    void onNext();
    void onPrevious();
    void onItemDoubleClicked(QListWidgetItem *item);
    void onPositionChanged(qint64 position);
    void onDurationChanged(qint64 duration);
    void onSliderMoved(int value);
    void onVolumeChanged(int value);
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

private:
    void setupUI();
    void loadTrack(const QString &filePath);
    void updateTimeLabels();

    QMediaPlayer *m_player;
    QAudioOutput *m_audioOutput;
    QListWidget *m_playlist;
    QPushButton *m_playButton;
    QPushButton *m_stopButton;
    QPushButton *m_nextButton;
    QPushButton *m_prevButton;
    QPushButton *m_addButton;
    QSlider *m_positionSlider;
    QSlider *m_volumeSlider;
    QLabel *m_currentTimeLabel;
    QLabel *m_totalTimeLabel;
    QLabel *m_volumeLabel;

    QStringList m_currentPlaylist;
    int m_currentIndex;
    bool m_isPlaying;
};

#endif
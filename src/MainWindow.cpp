#include "MainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>
#include <QMediaMetaData>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_currentIndex(-1)
    , m_isPlaying(false)
{
    // Criar player e audio output ANTES de configurar a UI (para que o slider de volume já tenha um alvo)
    m_player = new QMediaPlayer(this);
    m_audioOutput = new QAudioOutput(this);
    m_player->setAudioOutput(m_audioOutput);

    // Configurar a interface (agora o volume slider pode se conectar com segurança)
    setupUI();

    // Conectar sinais
    connect(m_player, &QMediaPlayer::positionChanged, this, &MainWindow::onPositionChanged);
    connect(m_player, &QMediaPlayer::durationChanged, this, &MainWindow::onDurationChanged);
    connect(m_player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::onMediaStatusChanged);
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    setWindowTitle("OpenMusic");
    resize(600, 400);

    // Widget central
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(central);

    // Playlist
    m_playlist = new QListWidget(this);
    m_playlist->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_playlist, &QListWidget::itemDoubleClicked, this, &MainWindow::onItemDoubleClicked);
    mainLayout->addWidget(m_playlist);

    // Controles de reprodução
    QHBoxLayout *controlLayout = new QHBoxLayout();

    m_addButton = new QPushButton("➕ Adicionar", this);
    connect(m_addButton, &QPushButton::clicked, this, &MainWindow::onAddFiles);
    controlLayout->addWidget(m_addButton);

    m_prevButton = new QPushButton("⏮", this);
    connect(m_prevButton, &QPushButton::clicked, this, &MainWindow::onPrevious);
    controlLayout->addWidget(m_prevButton);

    m_playButton = new QPushButton("▶", this);
    connect(m_playButton, &QPushButton::clicked, this, &MainWindow::onPlayPause);
    controlLayout->addWidget(m_playButton);

    m_stopButton = new QPushButton("⏹", this);
    connect(m_stopButton, &QPushButton::clicked, this, &MainWindow::onStop);
    controlLayout->addWidget(m_stopButton);

    m_nextButton = new QPushButton("⏭", this);
    connect(m_nextButton, &QPushButton::clicked, this, &MainWindow::onNext);
    controlLayout->addWidget(m_nextButton);

    controlLayout->addStretch();

    mainLayout->addLayout(controlLayout);

    // Slider de posição e tempo
    QHBoxLayout *positionLayout = new QHBoxLayout();
    m_currentTimeLabel = new QLabel("00:00", this);
    m_positionSlider = new QSlider(Qt::Horizontal, this);
    m_positionSlider->setRange(0, 0);
    connect(m_positionSlider, &QSlider::sliderMoved, this, &MainWindow::onSliderMoved);
    m_totalTimeLabel = new QLabel("00:00", this);
    positionLayout->addWidget(m_currentTimeLabel);
    positionLayout->addWidget(m_positionSlider);
    positionLayout->addWidget(m_totalTimeLabel);
    mainLayout->addLayout(positionLayout);

    // Volume
    QHBoxLayout *volumeLayout = new QHBoxLayout();
    m_volumeLabel = new QLabel("Volume:", this);
    m_volumeSlider = new QSlider(Qt::Horizontal, this);
    m_volumeSlider->setRange(0, 100);
    m_volumeSlider->setValue(50);
    // Define volume inicial
    m_audioOutput->setVolume(0.5);
    connect(m_volumeSlider, &QSlider::valueChanged, this, &MainWindow::onVolumeChanged);
    volumeLayout->addWidget(m_volumeLabel);
    volumeLayout->addWidget(m_volumeSlider);
    volumeLayout->addStretch();
    mainLayout->addLayout(volumeLayout);

    // Status
    QLabel *statusLabel = new QLabel("Pronto", this);
    mainLayout->addWidget(statusLabel);
}

void MainWindow::onAddFiles()
{
    QStringList files = QFileDialog::getOpenFileNames(this,
        "Adicionar músicas", QString(),
        "Arquivos de áudio (*.mp3 *.wav *.ogg *.flac *.m4a)");

    if (files.isEmpty()) return;

    m_currentPlaylist.append(files);
    for (const QString &file : files) {
        QFileInfo info(file);
        m_playlist->addItem(info.fileName());
    }
}

void MainWindow::onItemDoubleClicked(QListWidgetItem *item)
{
    if (!item) return;
    int index = m_playlist->row(item);
    if (index >= 0 && index < m_currentPlaylist.size()) {
        m_currentIndex = index;
        loadTrack(m_currentPlaylist.at(m_currentIndex));
        m_player->play();
        m_playButton->setText("⏸");
        m_isPlaying = true;
    }
}

void MainWindow::onPlayPause()
{
    if (m_currentPlaylist.isEmpty()) return;

    if (m_currentIndex < 0) {
        m_currentIndex = 0;
        loadTrack(m_currentPlaylist.first());
    }

    if (m_player->isPlaying()) {
        m_player->pause();
        m_playButton->setText("▶");
        m_isPlaying = false;
    } else {
        m_player->play();
        m_playButton->setText("⏸");
        m_isPlaying = true;
    }
}

void MainWindow::onStop()
{
    m_player->stop();
    m_playButton->setText("▶");
    m_isPlaying = false;
    m_positionSlider->setValue(0);
    m_currentTimeLabel->setText("00:00");
}

void MainWindow::onNext()
{
    if (m_currentPlaylist.isEmpty()) return;
    m_currentIndex = (m_currentIndex + 1) % m_currentPlaylist.size();
    loadTrack(m_currentPlaylist.at(m_currentIndex));
    m_player->play();
    m_playButton->setText("⏸");
    m_isPlaying = true;
}

void MainWindow::onPrevious()
{
    if (m_currentPlaylist.isEmpty()) return;
    m_currentIndex = (m_currentIndex - 1 + m_currentPlaylist.size()) % m_currentPlaylist.size();
    loadTrack(m_currentPlaylist.at(m_currentIndex));
    m_player->play();
    m_playButton->setText("⏸");
    m_isPlaying = true;
}

void MainWindow::loadTrack(const QString &filePath)
{
    m_player->setSource(QUrl::fromLocalFile(filePath));
    // Opcional: exibir metadados
    // QMediaMetaData meta = m_player->metaData();
    // setWindowTitle("OpenMusic - " + meta.value(QMediaMetaData::Title).toString());
}

void MainWindow::onPositionChanged(qint64 position)
{
    if (m_positionSlider->maximum() != 0) {
        m_positionSlider->setValue(static_cast<int>(position));
    }
    updateTimeLabels();
}

void MainWindow::onDurationChanged(qint64 duration)
{
    m_positionSlider->setRange(0, static_cast<int>(duration));
    updateTimeLabels();
}

void MainWindow::onSliderMoved(int value)
{
    m_player->setPosition(value);
}

void MainWindow::onVolumeChanged(int value)
{
    if (m_audioOutput) {
        m_audioOutput->setVolume(value / 100.0);
    }
}

void MainWindow::updateTimeLabels()
{
    qint64 current = m_player->position();
    qint64 total = m_player->duration();

    QTime currentTime = QTime::fromMSecsSinceStartOfDay(current);
    QTime totalTime = QTime::fromMSecsSinceStartOfDay(total);

    m_currentTimeLabel->setText(currentTime.toString("mm:ss"));
    m_totalTimeLabel->setText(totalTime.toString("mm:ss"));
}

void MainWindow::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::EndOfMedia) {
        onNext();
    }
}
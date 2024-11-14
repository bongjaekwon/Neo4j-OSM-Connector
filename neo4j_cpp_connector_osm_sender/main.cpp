#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QProcess>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include <QStringList>
#include <string>
#include <curl/curl.h>
#include <iostream>   // std::cerr, std::cout을 사용하기 위한 헤더 추가
#include <ostream>    // std::endl을 사용하기 위한 헤더 추가

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() {}

private slots:
    void startPodmanContainer();
    void loadOSMToNeo4j();

private:
    QProcess process;
    QLineEdit *osmFilePathInput;
    QLabel *statusLabel;
    std::string sendCypherQuery(const std::string& url, const std::string& query);
};

MainWindow::MainWindow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    osmFilePathInput = new QLineEdit(this);
    osmFilePathInput->setPlaceholderText("Enter path to OSM file (local path)");
    QPushButton *startButton = new QPushButton("Start Podman Container", this);
    QPushButton *importButton = new QPushButton("Import OSM to Neo4j", this);
    statusLabel = new QLabel("Status: Waiting...", this);

    layout->addWidget(osmFilePathInput);
    layout->addWidget(startButton);
    layout->addWidget(importButton);
    layout->addWidget(statusLabel);

    connect(startButton, &QPushButton::clicked, this, &MainWindow::startPodmanContainer);
    connect(importButton, &QPushButton::clicked, this, &MainWindow::loadOSMToNeo4j);
}

void MainWindow::startPodmanContainer() {
    // Podman 컨테이너 실행 명령어
    QString command = "podman run -d -v " + osmFilePathInput->text() + ":/var/lib/neo4j/import -e NEO4J_AUTH=neo4j/password --name neo4j3.5.2 neo4j:3.5.2";

    // 컨테이너가 이미 실행 중인지 확인
    QString checkCommand = "podman ps --filter 'name=neo4j3.5.2' --format '{{.Names}}'";

    process.start("/bin/bash", QStringList() << "-c" << checkCommand);
    process.waitForFinished();
    QString output = process.readAll().trimmed();

    if (output != "neo4j3.5.2") {
        // 컨테이너가 없으면 실행
        QStringList startArguments;
        startArguments << "-c" << command;
        process.startDetached("/bin/bash", startArguments);

        statusLabel->setText("Podman container started.");
    } else {
        statusLabel->setText("Podman container is already running.");
    }
}

std::string MainWindow::sendCypherQuery(const std::string& url, const std::string& query) {
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Neo4j URL 설정 (자신의 주소에 맞게 변경)
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        // HTTP 헤더 설정
        struct curl_slist *headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, "Authorization: Basic bmVvNGo6dGVzdA==");  // Basic 인증

        // Cypher 쿼리 데이터 생성
        std::string data = "{\"statements\" : [{\"statement\" : \"" + query + "\"}]}";

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "CURL 요청 실패: " << curl_easy_strerror(res) << std::endl;  // 에러 메시지 출력
            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
            curl_global_cleanup();
            return "Query failed";
        }

        std::cout << "Cypher Query Successfully Sent!" << std::endl;  // 성공 메시지 출력

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();
    return "Query sent successfully";
}

void MainWindow::loadOSMToNeo4j() {
    QString osmFilePath = osmFilePathInput->text();
    if (osmFilePath.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter the path to the OSM file.");
        return;
    }

    std::string url = "http://localhost:7476/db/data/transaction/commit";  // Neo4j의 REST API URL
    std::string cypherQuery = "CALL spatial.importOSM('file:///var/lib/neo4j/import/export')";  // 실제 OSM 파일 경로로 수정

    // Cypher 쿼리 전송
    std::string result = sendCypherQuery(url, cypherQuery);  // 수정된 부분
    statusLabel->setText(QString::fromStdString(result));
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.setWindowTitle("Neo4j OSM Importer");
    window.show();

    return app.exec();
}

#include "main.moc"

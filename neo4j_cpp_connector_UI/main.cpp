#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>
#include <curl/curl.h>
#include <string>
#include <iostream>

class MainWindow : public QWidget {
    Q_OBJECT  // Qt의 메타 객체 시스템을 사용하기 위한 필수 매크로

public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent) {
        // 레이아웃 설정
        QVBoxLayout *layout = new QVBoxLayout(this);

        // UI 요소 생성
        QLabel *label = new QLabel("Enter Cypher Query:", this);
        layout->addWidget(label);

        // Cypher 쿼리 입력란
        queryInput = new QLineEdit(this);
        layout->addWidget(queryInput);

        // 쿼리 전송 버튼
        sendButton = new QPushButton("Send Query", this);
        layout->addWidget(sendButton);

        // 결과 출력란
        resultOutput = new QTextEdit(this);
        resultOutput->setReadOnly(true);
        layout->addWidget(resultOutput);

        // 버튼 클릭 시 onSendButtonClicked() 슬롯 함수 호출
        connect(sendButton, &QPushButton::clicked, this, &MainWindow::onSendButtonClicked);
    }

private slots:
    void onSendButtonClicked() {
        // 텍스트 필드에서 쿼리 가져오기
        std::string query = queryInput->text().toStdString();
        
        // 쿼리 보내기
        std::string result = send_cypher_query(query);
        
        // 결과를 텍스트 출력 필드에 표시
        resultOutput->setText(QString::fromStdString(result));
    }

private:
    QLineEdit *queryInput;    // 쿼리 입력 필드
    QPushButton *sendButton;  // 쿼리 전송 버튼
    QTextEdit *resultOutput;  // 결과 출력 필드

    // Cypher 쿼리를 Neo4j 서버로 보내는 함수
    std::string send_cypher_query(const std::string& query) {
        CURL *curl;
        CURLcode res;
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();

        if(curl) {
            // Neo4j 서버의 URL로 설정
            curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:7476/db/data/transaction/commit");

            // 요청 헤더 설정
            struct curl_slist *headers = nullptr;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            headers = curl_slist_append(headers, "Authorization: Basic bmVvNGo6dGVzdA==");  // 기본 인증

            // 요청 데이터 준비 (JSON 형식으로 쿼리)
            std::string data = "{\"statements\" : [{\"statement\" : \"" + query + "\"}]}";

            // curl 옵션 설정
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());

            // 요청 수행
            res = curl_easy_perform(curl);
            if(res != CURLE_OK) {
                curl_easy_cleanup(curl);
                curl_slist_free_all(headers);
                curl_global_cleanup();
                return "Query failed: " + std::string(curl_easy_strerror(res));
            }

            curl_easy_cleanup(curl);
            curl_slist_free_all(headers);
        }

        curl_global_cleanup();
        return "Query sent successfully";
    }
};

// 메인 함수
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);  // Qt 애플리케이션 객체 생성

    MainWindow window;
    window.setWindowTitle("Neo4j C++ Connector");
    window.resize(400, 300);  // 창 크기 설정
    window.show();  // 창 표시

    return app.exec();  // Qt 애플리케이션 실행
}

// 'moc' 파일을 추가해야 합니다.
#include "main.moc"  // 이 라인 추가

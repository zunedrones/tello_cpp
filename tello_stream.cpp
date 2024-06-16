#include <chrono>
#include <iostream>

#include "opencv2/opencv.hpp"
#include "tello.h"
#include "unistd.h"

const char* const TELLO_STREAM_URL = "udp://0.0.0.0:11111";

using cv::CAP_FFMPEG;
using cv::imshow;
using cv::VideoCapture;
using cv::waitKey;
using tello::Tello;

int main() {
    // std::cout << cv::getBuildInformation() << "\n";

    Tello tello{};
    if (!tello.Bind()) {
        std::cerr << "Falha na conexão com o Tello." << "\n";
        return 0;
    }

    tello.SendCommand("streamon");
    std::cout << "Comando enviado: 'streamon' \n";
    std::optional<std::string> response;

    // Espera por uma resposta do comando "streamon"
    for (int i = 0; i < 10; i++) {
        response = tello.ReceiveResponse();
        if (response.has_value()) {
            std::cout << "Resposta: " << *response << "\n";
            break;
        }
        sleep(1);
    }

    if (!response.has_value()) {
        std::cerr << "Sem resposta para o comando 'streamon'." << "\n";
        return 0;
    }

    // Adicione uma pequena pausa para dar tempo ao drone de iniciar o stream
    sleep(2);

    VideoCapture capture("udp://0.0.0.0:11111", cv::CAP_FFMPEG);
    sleep(1);
    if (!capture.isOpened()) {
        std::cerr << "Erro ao abrir o stream de vídeo!" << "\n";
        return -1;
    }

    std::optional<std::string> bat;
    cv::namedWindow("Tello");
    cv::Mat frame;
    auto start = std::chrono::high_resolution_clock::now();
    int frame_count = 0;
    while (true) {
        frame_count++;
        tello.SendCommand("battery?");
        capture >> frame;
        if (!frame.empty()) {
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = end - start;
            double fps = frame_count / elapsed.count();

            std::string fps_text =
                "FPS: " + std::to_string(static_cast<int>(fps));
            cv::putText(frame, fps_text, cv::Point(10, 30),
                        cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
            /* bat = tello.ReceiveResponse();        
            std::string bat_text = "Battery: " + std::to_string((bat));
            cv::putText(frame, bat, cv::Point(450, 450),
                        cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 0, 255), 2);  */
            imshow("Tello", frame);
        } else {
            std::cerr << "Frame vazio recebido." << "\n";
        }
        if (waitKey(1) == 'q') {
            break;
        }
    }
    capture.release();
    cv::destroyAllWindows();
    return 0;
}

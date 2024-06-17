#include <chrono>
#include <iostream>

#include "opencv2/opencv.hpp"
#include "tello.h"
#include "unistd.h"

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


    cv::VideoCapture capture("udp://0.0.0.0:11111", cv::CAP_FFMPEG);
    if (!capture.isOpened()) {
        std::cerr << "Erro ao abrir o stream de vídeo!" << "\n";
        return -1;
    }

    std::optional<std::string> bat;
    cv::Mat frame;
    cv::Mat resized_frame;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
    int frame_count = 0;
    int fps;
    bool start_timing = false;
    while (true) {
        tello.SendCommand("battery?");
        capture >> frame;
        if (!frame.empty()) {
            if (!start_timing) {
                cv::namedWindow("Tello");
                start = std::chrono::high_resolution_clock::now();
                start_timing = true;
            } else {
                frame_count++;
                auto end = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed = end - start;
    
            if (elapsed.count() >= 1.0) { 
                fps = frame_count / elapsed.count();
                frame_count = 0;
                start = std::chrono::high_resolution_clock::now();
            }
            cv::resize(frame, resized_frame, cv::Size(544, 306));
            std::string fps_text =
                "FPS: " + std::to_string(static_cast<int>(fps));
            cv::putText(resized_frame, fps_text, cv::Point(10, 30),
                        cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
            bat = tello.ReceiveResponse();
            cv::putText(resized_frame, "Battery: " + *bat + "%", cv::Point(330, 300),
                        cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(0, 255, 0), 2);
            cv::imshow("Tello", resized_frame);
            }
        }
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }
    capture.release();
    cv::destroyAllWindows();
    return 0;
}

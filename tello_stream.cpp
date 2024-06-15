#include <iostream>
#include "unistd.h"

#include "tello.h"
#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/videoio.hpp"

const char* const TELLO_STREAM_URL = "udp://0.0.0.0:11111";

using tello::Tello;
using cv::CAP_FFMPEG;
using cv::imshow;
using cv::VideoCapture;
using cv::waitKey;

int main()
{
    //std::cout << cv::getBuildInformation() << std::endl;

    Tello tello{};
    if (!tello.Bind())
    {
        std::cerr << "Falha na conexão com o Tello." << std::endl;
        return 0;
    }

    tello.SendCommand("streamon");
    std::cout << "Comando enviado: 'streamon' \n";
    std::optional<std::string> response;
    
    // Espera por uma resposta do comando "streamon"
    for (int i = 0; i < 10; i++) {
        response = tello.ReceiveResponse();
        if (response.has_value()) {
            std::cout << "Resposta: " << *response << std::endl;
            break;
        }
        sleep(1);
    }

    if (!response.has_value()) {
        std::cerr << "Sem resposta para o comando 'streamon'." << std::endl;
        return 0;
    }

    // Adicione uma pequena pausa para dar tempo ao drone de iniciar o stream
    sleep(2);

    VideoCapture capture("udp://0.0.0.0:11111", cv::CAP_FFMPEG);
    sleep(1);
     if (!capture.isOpened()) {
        std::cerr << "Erro ao abrir o stream de vídeo!" << std::endl;
        return -1;
    } 

    cv::namedWindow("Tello");
    while (true)
    {
        cv::Mat frame;
        capture >> frame;
        if (!frame.empty()) {
            imshow("Tello", frame);
        } else {
            std::cerr << "Frame vazio recebido." << std::endl;
        }
        if (waitKey(1) == 'q')
        {
            break;
        }
    }
    return 0;
}

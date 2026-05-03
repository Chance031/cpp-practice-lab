#include <iostream> // std::cout을 사용하기 위한 헤더
#include <chrono>   // 시간을 측정하기 위한 헤더
#include <thread>   // sleep_for()를 사용하기 위한 헤더

int main()
{
    // steady_clock은 일정하게 흐르는 시계를 제공한다.
    // now()는 현재 시각을 가져온다.
    auto startTime = std::chrono::steady_clock::now();

    while (true)
    {
        // 현재 시각을 저장한다.
        auto currentTime = std::chrono::steady_clock::now();

        // currentTime - startTime은 시작 후 얼마나 시간이 지났는지 계산한다.
        // duration_cast는 시간 차이를 원하는 단위로 변환한다.
        // chrono::seconds는 초 단위를 뜻한다.
        auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);

        // 지금까지 지난 시간을 같은 줄에 계속 출력한다.
        // \r은 커서를 현재 줄의 맨 앞으로 이동시킨다.
        // std::flush는 출력 버퍼의 내용을 바로 화면에 반영한다.
        std::cout << "\rElapsed Time: " << elapsedTime.count() << " second(s)" << std::flush;

        // 5초가 지나면 반복문을 종료한다.
        if (elapsedTime.count() >= 5)
            break;

        // 반복문이 너무 빠르게 실행되지 않도록 잠시 대기한다.
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    std::cout << "\nTimer Ended.\n";

    return 0;
}

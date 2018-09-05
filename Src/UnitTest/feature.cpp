#include <Src/UnitTest/feature.h>
#include <Execption.h>
#ifdef WIN32
#include <windows.h>
#include <WheatyExceptionReport.h>
#endif
typedef struct st_mysql_stmt MYSQL_STMT;

Promise::Promise()
{
}

Promise::~Promise()
{
}

#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <iostream>
#include <chrono>

void accumulate(std::vector<int>::iterator first,
	std::vector<int>::iterator last,
	std::promise<int> accumulate_promise)
{
	int sum = std::accumulate(first, last, 0);
	accumulate_promise.set_value(sum);  // 提醒 future
}

void do_work(std::promise<void> barrier)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	barrier.set_value();
}


void main()
{
	std::vector<int> numbers = { 1, 2, 3, 4, 5, 6 };
	std::promise<int> accumulate_promise;
	std::future<int> accumulate_future = accumulate_promise.get_future();
	std::thread work_thread(accumulate, numbers.begin(), numbers.end(),
		std::move(accumulate_promise));
	accumulate_future.wait();  // 等待结果
	accumulate_future.get();
	std::cout << "result=" << accumulate_future.get() << '\n';
	work_thread.join();  // wait for thread completion

						 // 演示用 promise<void> 在线程间对状态发信号
	std::promise<void> barrier;
	std::future<void> barrier_future = barrier.get_future();
	std::thread new_work_thread(do_work, std::move(barrier));
	barrier_future.wait();
	new_work_thread.join();
	Promise* p = new Promise();
	delete p;
	delete p;
}
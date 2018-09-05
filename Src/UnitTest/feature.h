#include <future>

class Promise
{
public:
	Promise();
	~Promise();

private:


	std::future<int> testIntFuture;
	std::promise<int> testIntPromise;
};
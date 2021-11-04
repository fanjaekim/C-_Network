#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <memory>
#include <mutex>

using namespace std;

const int MaxCount = 300000;
const int ThreadCount = 4;

bool IsPrimeNumber(int number)
{
	if (number == 1)
	{
		return false;
	}
	if (number == 2 || number == 3)
	{
		return true;
	}
	for (int i = 2; i < number - 1; i++)
	{
		if ((number % i) == 0)
		{
			return false;
		}
	}
	return true;
}

void PrintNumbers(const vector<int>& primes)
{
	for (int v : primes)
	{
		cout << v << endl;
	}
}

int main()
{
	int num = 1;
	recursive_mutex num_mutex;
	vector<int> primes;
	recursive_mutex primes_mutex;

	auto t0 = chrono::system_clock::now();

	vector<shared_ptr<thread> > threads;

	for (int i = 0; i < ThreadCount; i++)
	{
		shared_ptr<thread> thread(new thread([&]() {
			// �������� �����Լ�
			// ���� ������ �� ������ ������ ����.
			while (true)
			{
				int n;
				{ // ��(lock)�� �Ͽ� �ٸ� �����尡 ��� ���·� ��ȯ.
					lock_guard<recursive_mutex> num_lock(num_mutex);
					n = num;
					num++;
				}
				if (n >= MaxCount)
				{
					break;
				}

				if (IsPrimeNumber(n))
				{
					lock_guard<recursive_mutex> primes_lock(primes_mutex);
					primes.push_back(n);
				}
			}
		}));
		// ������ ��ü�� �ϴ� ���� �ִ´�.
		threads.push_back(thread);
	}

	// ��� �����尡 ���� ��ĥ�� ���� ��ٸ���.
	for (auto thread : threads)
	{
		thread->join();
	}

	auto t1 = chrono::system_clock::now();
	auto duration = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
	cout << "Took " << duration << " milliseconds." << endl;

}
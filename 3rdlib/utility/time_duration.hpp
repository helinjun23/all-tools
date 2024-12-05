#pragma once

#include <chrono>

namespace utils {
	using namespace std;
	using namespace chrono;

	class TimeDuration
	{
		using time_now = high_resolution_clock;
		using sys_clock_t = system_clock::time_point;	 // ����ʱ�䣬���ױ�ϵͳʱ��Ӱ��
		using steady_clock_t = steady_clock::time_point; // �ȶ�ʱ�䣬��������
		using high_clock_t = high_resolution_clock::time_point;
	public:
		TimeDuration() {
			start_time = time_now::now();
		}
		// ���»�ȡ��ʼ��ʱ��
		virtual long long update() {
			start_time = time_now::now();
			return start_time.time_since_epoch().count();
		}
		virtual long long duration_us() {
			stop_time = time_now::now();
			return duration_cast<microseconds>(stop_time - start_time).count();
		}
		// ���ؼ��ʱ��s
		virtual long long duration_ms() {
			stop_time = time_now::now();
			return duration_cast<milliseconds>(stop_time - start_time).count();
		}
		virtual double duration_s() {
			stop_time = time_now::now();
			return duration_cast<duration<double>>(stop_time - start_time).count();
		}
		virtual ~TimeDuration() {
		}

	private:
		high_clock_t start_time;
		high_clock_t stop_time;
	};
}





#include "animation_curve.h"

#include <cmath>

#include "debug_log.h"

namespace component {

	int CAT_AnimationCurve::factorial(int n) {
		if (n == 0) {
			return 1;
		}

		int ans = 1;
		for (int i = n; i > 0; i--) {
			ans *= i;
		}

		return ans;

	}

	int CAT_AnimationCurve::biCoe(int n, int k) {
		if (n < k) {
			return -1;
		}
		return factorial(n) / (factorial(k) * factorial(n-k));
	}

	float CAT_AnimationCurve::bernstein(int n, int i, float t) {
		return biCoe(n, i) * std::pow((1 - t), (n - i)) * std::pow(t, i);
	}


	float CAT_AnimationCurve::bezierCurve(std::vector<float> points, float t) {
		float Gt = 0;
		int n = points.size() - 1;

		

		for (int i = 0; i < points.size(); i++) {
			//debug::debugLog("Point : %f\n", points[i]);
			Gt += points[i] * bernstein(n, i, t);
		}

		return Gt;

	}


	CAT_AnimationCurve::CAT_AnimationCurve(CAT_AnimationCurve::ComponentInitializer* cInit_ptr) {
		for (auto anim_data : cInit_ptr->key_points_data->nexts["animation"][0]->nexts["key"]) {
			this->key_points.push_back({ std::stoi(anim_data->nexts["time"][0]->item), std::stod(anim_data->nexts["value"][0]->item) });
		}
		this->loop = cInit_ptr->loop;
		if (this->key_points.size() < 2) {
			/* —áŠOˆ— */
		}
		for (int i = 0; i < this->key_points.size() - 1; i++) {
			float first = this->key_points[i].second;
			float second = this->key_points[i + 1].second;
			float mid1 = first;
			float mid2 = second;
			std::vector<float> points = { first, mid1, mid2,second };
			float time_part = (float)1 / (this->key_points[i + 1].first / 10);
			
			for (int j = 0; j < this->key_points[i + 1].first / 10; j++) {
				this->frame_data[j * 10] = bezierCurve(points, time_part * j);
				//debug::debugLog("Log : %f\n", this->frame_data[j * 10]);
			}

			this->sum_time += this->key_points[i + 1].first;
		}

		this->frame_data[this->sum_time / 10 * 10] = this->key_points[this->key_points.size() - 1].second;
	}

	void CAT_AnimationCurve::update() {
		*(this->set_ptr) = this->frame_data[std::clamp((time / 10) * 10, 0 ,sum_time / 10 * 10)];
		//debug::debugLog("Key Value : %f \n", *(this->set_ptr));
	}

	void CAT_AnimationCurve::gain(int delta_time) {
		time += delta_time;

		if (time > this->sum_time) {
			if (loop > 0) {
				time %= this->sum_time;
			}
			else {
				time = this->sum_time / 10 * 10;
			}
		}
	}

	void CAT_AnimationCurve::reset() {
		time = 0;
	}

}
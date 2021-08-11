#include "mvCamera.h"
#include "mvGraphics.h"
#include <algorithm>

namespace DearPy3D {

	template<typename T>
	static T wrap_angle(T theta) noexcept
	{
		constexpr T twoPi = (T)2 * (T)PI_D;
		const T mod = (T)fmod(theta, twoPi);
		if (mod > (T)PI_D)
			return mod - twoPi;
		else if (mod < -(T)PI_D)
			return mod + twoPi;
		return mod;
	}

	mvCamera::mvCamera(float width, float height, glm::vec3 homePos)
		:
		_pos(homePos),
		_pitch(0.0f),
		_yaw(0.0f),
		_width(width),
		_height(height)
	{

	}

	glm::mat4 mvCamera::getMatrix() const
	{
		glm::mat4 roll_pitch_yaw = glm::yawPitchRoll(_yaw, _pitch, 0.0f);

		glm::vec4 forward_base_vector = { 0.0f, 0.0f, 1.0f, 0.0f };

		glm::vec4 look_vector = roll_pitch_yaw * forward_base_vector;

		glm::vec3 lpos = { look_vector.x, look_vector.y, look_vector.z };

		glm::vec3 look_target = _pos + lpos;

		glm::mat4 camera_matrix = glm::lookAt(_pos, look_target, glm::vec3{ 0.0f, -1.0f, 0.0f });

		return camera_matrix;
	}

	glm::mat4 mvCamera::getProjection() const
	{
		return glm::perspective(glm::radians(45.0f), _width / _height, 0.1f, 400.0f);
	}

	void mvCamera::rotate(float dx, float dy)
	{
		_yaw = wrap_angle(_yaw + dx * s_rotationSpeed);
		_pitch = std::clamp(_pitch + dy * s_rotationSpeed, 0.995f * -PI / 2.0f, 0.995f * PI / 2.0f);
	}

	void mvCamera::translate(float dx, float dy, float dz)
	{
		glm::mat4 roll_pitch_yaw = glm::yawPitchRoll(_yaw, _pitch, 0.0f);
		glm::mat4 scale = glm::scale(glm::vec3{ s_travelSpeed, s_travelSpeed, s_travelSpeed });
		glm::vec4 translation = glm::vec4{ dx, dy, dz, 0.0f };

		translation = (roll_pitch_yaw * scale) * translation;

		_pos = {
			_pos.x + translation.x,
			_pos.y + translation.y,
			_pos.z + translation.z
		};
	}

}
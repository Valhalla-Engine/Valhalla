#pragma once

#include "yaml-cpp/yaml.h"
#include "Valhalla/Core/Math/Math.h"

namespace YAML
{
	template<>
	struct convert<glm::vec2>
	{
		static Node node(const glm::vec2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, glm::vec2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float_t>();
			rhs.y = node[1].as<float_t>();
			return true;
		}
	};

	template<>
	struct convert<DirectX::XMFLOAT2>
	{
		static Node node(const DirectX::XMFLOAT2& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			return node;
		}

		static bool decode(const Node& node, DirectX::XMFLOAT2& rhs)
		{
			if (!node.IsSequence() || node.size() != 2)
				return false;

			rhs.x = node[0].as<float_t>();
			rhs.y = node[1].as<float_t>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec3>
	{
		static Node node(const glm::vec3& rhs)
		{
			Node node;

			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);

			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float_t>();
			rhs.y = node[1].as<float_t>();
			rhs.z = node[2].as<float_t>();
			return true;
		}
	};

	template<>
	struct convert<DirectX::XMFLOAT3>
	{
		static Node node(const DirectX::XMFLOAT3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);

			return node;
		}

		static bool decode(const Node& node, DirectX::XMFLOAT3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float_t>();
			rhs.y = node[1].as<float_t>();
			rhs.z = node[2].as<float_t>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node node(const glm::vec4& rhs)
		{
			Node node;

			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float_t>();
			rhs.y = node[1].as<float_t>();
			rhs.z = node[2].as<float_t>();
			rhs.w = node[3].as<float_t>();
			return true;
		}
	};

	template<>
	struct convert<DirectX::XMFLOAT4>
	{
		static Node node(const DirectX::XMFLOAT4& rhs)
		{
			Node node;

			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, DirectX::XMFLOAT4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float_t>();
			rhs.y = node[1].as<float_t>();
			rhs.z = node[2].as<float_t>();
			rhs.w = node[3].as<float_t>();
			return true;
		}
	};

	template<>
	struct convert<glm::quat>
	{
		static Node node(const glm::quat& rhs)
		{
			Node node;
			node.push_back(rhs.w);
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::quat& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.w = node[0].as<float_t>();
			rhs.x = node[1].as<float_t>();
			rhs.y = node[2].as<float_t>();
			rhs.z = node[3].as<float_t>();
			return true;
		}
	};

	template<>
	struct convert<DirectX::XMQUAT>
	{
		static Node node(const DirectX::XMQUAT& rhs)
		{
			Node node;
			node.push_back(rhs.w);
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, DirectX::XMQUAT& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.w = node[0].as<float_t>();
			rhs.x = node[1].as<float_t>();
			rhs.y = node[2].as<float_t>();
			rhs.z = node[3].as<float_t>();
			return true;
		}
	};
}

namespace Valhalla
{
	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const DirectX::XMFLOAT2& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const DirectX::XMFLOAT3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const DirectX::XMFLOAT4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const glm::quat& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.w << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	inline YAML::Emitter& operator<<(YAML::Emitter& out, const DirectX::XMQUAT& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.w << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}
}
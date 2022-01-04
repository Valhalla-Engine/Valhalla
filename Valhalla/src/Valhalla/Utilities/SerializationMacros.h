#pragma once

#define VH_SERIALIZE_PROPERTY(propName, propVal, outputNode) outputNode << YAML::Key << #propName << YAML::Value << propVal

#define VH_DESERIALIZE_PROPERTY(propName, destination, node, defaultValue) destination = node[#propName] ? node[#propName].as<decltype(defaultValue)>() : defaultValue
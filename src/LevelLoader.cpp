#include "LevelLoader.h"

void operator >> (const YAML::Node& obstacleNode, ObstacleData& obstacle)
{
   obstacle.m_type = obstacleNode["type"].as<std::string>();
   obstacle.m_position.x = obstacleNode["position"]["x"].as<float>();
   obstacle.m_position.y = obstacleNode["position"]["y"].as<float>();
   obstacle.m_rotation = obstacleNode["rotation"].as<double>();
}

void operator >> (const YAML::Node& targetNode, Targetdata& target)
{
	target.m_type = targetNode["type"].as<std::string>();
	target.m_position.x = targetNode["position"]["x"].as<float>();
	target.m_position.y = targetNode["position"]["y"].as<float>();
	target.m_rotation = targetNode["rotation"].as<double>();
}

void operator >> (const YAML::Node& backgroundNode, BackgroundData& background)
{
   background.m_fileName = backgroundNode["file"].as<std::string>();
}

void operator >> (const YAML::Node& tankNode, TankData& tank)
{
	tank.m_position.x = tankNode["position"]["x"].as<float>();
	tank.m_position.y = tankNode["position"]["y"].as<float>();
}


void operator >> (const YAML::Node& levelNode, LevelData& level)
{
   levelNode["background"] >> level.m_background;

   levelNode["tank"] >> level.m_tank;

   const YAML::Node& obstaclesNode = levelNode["obstacles"].as<YAML::Node>();
   const YAML::Node& targetNode = levelNode["targets"].as<YAML::Node>();

   for (unsigned i = 0; i < obstaclesNode.size(); ++i)
   {
	  ObstacleData obstacle;
	  obstaclesNode[i] >> obstacle;
	  level.m_obstacles.push_back(obstacle);
   }

   for (unsigned i = 0; i < targetNode.size(); i++)
   {
	   Targetdata target;
	   targetNode[i] >> target;
	   level.m_target.push_back(target);
   }
}

LevelLoader::LevelLoader()
{
}

bool LevelLoader::load(int nr, LevelData& level)
{
   std::stringstream ss;
   ss << "./resources/levels/level";
   ss << nr;
   ss << ".yaml";

   try
   {
	  YAML::Node baseNode = YAML::LoadFile(ss.str());
	  if (baseNode.IsNull())
	  {
		  std::string message("file: " + ss.str() + " not found");
		  throw std::exception(message.c_str());
	  }
	  baseNode >> level;
   }

   catch(YAML::ParserException& e)
   {
       std::cout << e.what() << "\n";
       return false;
   }

   catch (std::exception& e)
   {
	   std::cout << e.what() << "\n";
	   return false;
   }
   return true;
}

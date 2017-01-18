#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP

#include <string>
#include <boost/property_tree/info_parser.hpp>
#include <boost/property_tree/ptree.hpp>

typedef boost::property_tree::ptree ptree;

class Configuration {

	private:
		ptree _config_tree;
		bool _config_loaded;

	public:
		// Common settings
		std::string inputDataFile;
		std::string solutionFile;
		std::string solver;
		int maxExecTime;

		// GRASP settings
		float alpha;

		// BRKGA settings
		float fIndividuals;
		float pElites;
		float pMutants;
		float pInheritanceElite;

		// Local Search settings
		bool localSearch;
		std::string neighborhoodStrategy;
		std::string policy;

	public:
		Configuration() :
			_config_tree(),
			_config_loaded( false )
		{
		}

		void load( std::string configFilename, std::string inputFilename, std::string solutionFilename )
		{
			if ( !_config_loaded && !configFilename.empty() ) {
				boost::property_tree::read_info( configFilename, _config_tree );
				_config_loaded = true;

				inputDataFile = inputFilename;
				solutionFile = solutionFilename;

				solver =               _config_tree.get<std::string>("solver");
				maxExecTime =          _config_tree.get<int>("maxExecTime");

				alpha =                _config_tree.get<float>("GRASP.alpha");

				fIndividuals =         _config_tree.get<float>("BRKGA.fIndividuals");
				pElites =              _config_tree.get<float>("BRKGA.pElites");
				pMutants =             _config_tree.get<float>("BRKGA.pMutants");
				pInheritanceElite =    _config_tree.get<float>("BRKGA.pInheritanceElite");

				localSearch =          _config_tree.get<bool>("localSearch");
				neighborhoodStrategy = _config_tree.get<std::string>("localSearch.neighborhoodStrategy");
				policy =               _config_tree.get<std::string>("localSearch.policy");
			}
		}
};

#endif // CONFIGURATION_HPP

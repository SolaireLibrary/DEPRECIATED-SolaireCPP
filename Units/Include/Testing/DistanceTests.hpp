#ifndef SOLAIRE_UNITS_DISTANCE_TEST_HPP
#define SOLAIRE_UNITS_DISTANCE_TEST_HPP

//Copyright 2015 Adam Smith
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.

#include "..\Distance.hpp"
#include "Testing.hpp"

namespace Solaire{ namespace Units { namespace Testing{

	static bool DistanceTest00(){
		// Create converter
		Distance<double> converter;

		// Initialise with a value
		converter.Metres = 100.0;

		// Check that the same value is returned
		if(converter.Metres != 100.0) return false;

		// Test that the unit is converted correctly
		double threshold = 1.0;
		double expectedResult = 0.0;
		double actualResult = 0.0;

		expectedResult = 3937.00787;
		actualResult = converter.Inches;
		if(PercentageDifference<double>(expectedResult, actualResult) > threshold) return false;
		
		expectedResult = 328.08399;
		actualResult = converter.Feet;
		if(PercentageDifference<double>(expectedResult, actualResult) > threshold) return false;

		expectedResult = 109.36133;
		actualResult = converter.Yards;
		if(PercentageDifference<double>(expectedResult, actualResult) > threshold) return false;

		expectedResult = 0.0621371192;
		actualResult = converter.Miles;
		if(PercentageDifference<double>(expectedResult, actualResult) > threshold) return false;

		expectedResult = 4.97096954;
		actualResult = converter.Chains;
		if(PercentageDifference<double>(expectedResult, actualResult) > threshold) return false;
		
		expectedResult = 19.8838782;
		actualResult = converter.Rods;
		if(PercentageDifference<double>(expectedResult, actualResult) > threshold) return false;
		
		//! \TODO Test Links

		return true;
	}

	static bool DistanceTest01(){
		// Create converter
		Distance<double> converter;

		// Initialise with a value
		converter.Miles = 5.0;

		// Check that the same value is returned
		if(converter.Miles != 5.0) return false;

		// Test that the unit is converted correctly
		double threshold = 1.0;
		double expectedResult = 0.0;
		double actualResult = 0.0;

		expectedResult = 40.0;
		actualResult = converter.Furlongs;
		if(PercentageDifference<double>(expectedResult, actualResult) > threshold) return false;

		//! \TODO Test LeagueUK & LeaugesUS

		expectedResult = 4.34488;
		actualResult = converter.NauticalMiles;
		if(PercentageDifference<double>(expectedResult, actualResult) > threshold) return false;

		expectedResult = 1.44829374;
		actualResult = converter.NauticalLeagues;
		if(PercentageDifference<double>(expectedResult, actualResult) > threshold) return false;
		
		return true;
	}

	static bool DistanceTest02(){
		// Create converter
		Distance<double> converter;

		// Initialise with a value
		converter.Lightyears = 1.0;

		// Check that the same value is returned
		if(converter.Lightyears != 1.0) return false;

		// Test that the unit is converted correctly
		double threshold = 1.0;
		double expectedResult = 0.0;
		double actualResult = 0.0;

		expectedResult = 9.4605284e+15;
		actualResult = converter.Metres;
		if(PercentageDifference<double>(expectedResult, actualResult) > threshold) return false;
		
		expectedResult = 0.306594845;
		actualResult = converter.Parsecs;
		if(PercentageDifference<double>(expectedResult, actualResult) > threshold) return false;

		expectedResult = 9.4605284e+12;
		actualResult = converter.Kilometres;
		if(PercentageDifference<double>(expectedResult, actualResult) > threshold) return false;
		
		return true;
	}
	
	static bool DistanceTest03(){
		// Create converter
		Distance<float> converter;

		// Initialise with a value
		converter.Miles = 5.0;

		// Check that the same value is returned
		if(converter.Miles != 5.0) return false;

		// Test that the unit is converted correctly
		float threshold = 1.0;
		float expectedResult = 0.0;
		float actualResult = 0.0;

		expectedResult = 40.0;
		actualResult = converter.Furlongs;
		if(PercentageDifference<double>(expectedResult, actualResult) > threshold) return false;

		//! \TODO Test LeagueUK & LeaugesUS

		expectedResult = 4.34488;
		actualResult = converter.NauticalMiles;
		if(PercentageDifference<double>(expectedResult, actualResult) > threshold) return false;

		expectedResult = 1.44829374;
		actualResult = converter.NauticalLeagues;
		if(PercentageDifference<double>(expectedResult, actualResult) > threshold) return false;
		
		return true;
	}
	
	static bool DistanceTest04(){
		// Create converter
		Distance<int> converter;

		// Initialise with a value
		converter.Miles = 5;

		// Check that the same value is returned
		if(converter.Miles != 5) return false;

		// Test that the unit is converted correctly
		double threshold = 1.0;
		double expectedResult = 0.0;
		double actualResult = 0.0;

		expectedResult = 40.0;
		actualResult = converter.Furlongs;
		if(PercentageDifference<double>(expectedResult, actualResult) > threshold) return false;

		//! \TODO Test LeagueUK & LeaugesUS

		expectedResult = std::floor(4.34488);
		actualResult = converter.NauticalMiles;
		if(PercentageDifference<double>(expectedResult, actualResult) > threshold) return false;

		expectedResult = std::floor(1.44829374);
		actualResult = converter.NauticalLeagues;
		if(PercentageDifference<double>(expectedResult, actualResult) > threshold) return false;
		
		return true;
	}
}}}


#endif
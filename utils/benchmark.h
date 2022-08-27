#pragma once

#include <GLFW/glfw3.h>
#include <iostream>
#include <string.h>
#include <boost/lexical_cast.hpp>

class Benchmark {
public:
	Benchmark( std::string _title );

	void startBenchmark( std::string _title );

	void endBenchmark();

private:
	std::string title = "";
	float startTime = 0;
	float endTime = 0;

	void printResult();

};
#include "benchmark.h"

Benchmark::Benchmark( std::string _title ) {
	startBenchmark( _title );
};

void Benchmark::startBenchmark( std::string _title ) {
	title = _title;
	startTime = glfwGetTime();
}

void Benchmark::endBenchmark() {
	if (startTime == 0) {
		std::cout << "Timer for " + title + " not started!\n";
		return;
	}

	endTime = glfwGetTime();
	printResult();

	startTime = 0;
	endTime = 0;
};

void Benchmark::printResult() {
	float time = (endTime - startTime);
	std::cout << "Benchmark \"" + title + "\" took " + boost::lexical_cast<std::string>(time) << "s\n";
}
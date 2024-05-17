#include <sycl/sycl.hpp>
#include <iostream>

#include <Window/Application.h>

#include <Simulation/Simulation.h>

using namespace sycl;

// Create an exception handler for asynchronous SYCL exceptions
static auto exception_handler = [](sycl::exception_list e_list) {
	for (std::exception_ptr const &e : e_list) {
		try {
			std::rethrow_exception(e);
		}
		catch (std::exception const &e) {
			std::terminate();
		}
	}
};

int main(int argc, char* argv[]) {
	/*
	device device;

	try {
		queue q(device, exception_handler);

		// Print out the device information used for the kernel code.
		std::cout << "Running on device: " << q.get_device().get_info<info::device::name>() << "\n";
	} catch (exception const &e) {
		std::cout << e.what() << std::endl;
		std::terminate();
	}
	*/

	Application::Init();
	{
		std::normal_distribution<float>::param_type massParams{ 5.f, 1.f };
		std::normal_distribution<float>::param_type hParams{ 0.f, 50.f };
		std::normal_distribution<float>::param_type vParams{ 0.f, 5.f };
		std::normal_distribution<float>::param_type speedParams{ 0.f, 0.0001f };

		auto generator = Random::PlanetaryDiskGenerator<
			std::normal_distribution<float>,
			std::normal_distribution<float>,
			std::normal_distribution<float>,
			std::normal_distribution<float>
		> (massParams, hParams, vParams, speedParams);

		Application::SetSimulation(
			std::make_unique<Simulation::Instance>(
				std::make_unique<Simulation::World>(Simulation::World::RandomWorld(35, generator)),
				Duration(1.f)
			)
		);
	}
	Application::Enter();

	return 0;
}
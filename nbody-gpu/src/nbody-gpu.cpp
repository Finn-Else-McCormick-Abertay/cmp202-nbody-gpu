#include <sycl/sycl.hpp>
#include <iostream>

#include <Application/Application.h>

#include <Simulation/Simulation.h>

using namespace sycl;

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
		float starMass = 10'000.f;

		float diskSize = 100'000.f;

		Random::normal::param_type massParams{ 1000.f, 600.f };
		Random::normal::param_type hParams{ 0.f, diskSize };
		Random::normal::param_type vParams{ 0.f, 5.f };
		Random::normal::param_type speedParams{ 10.f, 0.02f };

		auto generator = Random::DiskGenerator(massParams, hParams, hParams, vParams, speedParams);

		Application::SetSimulation(
			std::make_unique<Simulation::Instance>(
				std::make_unique<Simulation::World>(
					Simulation::World::Combine(
						Simulation::World::CentralStar(starMass),
						Simulation::World::RandomWorld(500, generator)
					)),
				Duration(1.f)
			)
		);
	}
	Application::Enter();

	return 0;
}
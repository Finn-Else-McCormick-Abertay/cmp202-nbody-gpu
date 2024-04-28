#include <sycl/sycl.hpp>

#include <Window/Application.h>

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
	auto selector = default_selector_v;

	try {
		queue q(selector, exception_handler);

		// Print out the device information used for the kernel code.
		std::cout << "Running on device: " << q.get_device().get_info<info::device::name>() << "\n";
	} catch (exception const &e) {
		std::terminate();
	}
	*/

	Application::Init();
	Application::Enter();

	return 0;
}
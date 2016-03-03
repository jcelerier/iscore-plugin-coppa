#include "MinuitDevice.hpp"
#include "MinuitAddress.hpp"
#include "MinuitNode.hpp"
#include "MinuitProtocol.hpp"

static void test()
{
  using namespace coppa;
  using namespace coppa::ossia_wrapper;
  using namespace coppa::ossia_wrapper::Minuit;
  auto proto = std::make_shared<coppa::ossia_wrapper::Minuit::MinuitClient>("127.0.0.1", 12345, 4356);
  Device<MinuitClient> dev(proto);
}

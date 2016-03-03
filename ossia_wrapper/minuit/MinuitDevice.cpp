#include "MinuitDevice.hpp"
#include "MinuitAddress.hpp"
#include "MinuitNode.hpp"
#include "MinuitProtocol.hpp"

static void test()
{
    using namespace coppa;
    using namespace coppa::minuit_wrapper;
  auto proto = std::make_shared<coppa::minuit_wrapper::MinuitClient>("127.0.0.1");
  coppa::minuit_wrapper::Device<MinuitClient, MinuitClient::protocol_t> dev(proto);
}

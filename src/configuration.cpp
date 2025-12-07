#include "configuration.hpp"

using namespace MPingSender;

MPingSender::Configuration::Configuration(const std::span<char *> args)
{
    boost::program_options::options_description options{"Configuration"};
    // clang-format off
    options.add_options()
      ("help", "Help screen")
      ("log-level", boost::program_options::value<boost::log::trivial::severity_level>(&this->_log_level)->default_value(boost::log::trivial::info), "Log level. Defaults to info.")
      ("bind-address", boost::program_options::value<std::string>(&this->_bind_address)->required(), "Address to bind")
      ("bind-port", boost::program_options::value<uint16_t>(&this->_bind_port)->required(), "Port to bind")
      ("address", boost::program_options::value<std::string>(&this->_address)->default_value("ff2e::42"), "Address to send packets to. Defaults to ff2e::42")
      ("port", boost::program_options::value<uint16_t>(&this->_port)->default_value(4321), "Port to send packets to")
      ("ttl", boost::program_options::value<uint8_t>(&this->_ttl)->default_value(64), "TTL/Hoplimit the packets sends with (0-255). Defaults to 64.")
      ("interface-name", boost::program_options::value<std::string>(&this->_interface_name)->required(), "TTL/Hoplimit the packets sends with")
    ;
    // clang-format on

    boost::program_options::command_line_parser parser{
        static_cast<int>(args.size()), args.data()};
    parser.options(options).allow_unregistered().style(
        boost::program_options::command_line_style::allow_long |
        boost::program_options::command_line_style::allow_sticky |
        boost::program_options::command_line_style::long_allow_next);
    boost::program_options::parsed_options parsed_options = parser.run();

    boost::program_options::variables_map vm;
    store(parsed_options, vm);

    this->_help = vm.contains("help");
    if (this->_help)
    {
        std::clog << "mping-sender" << std::endl
                  << std::endl
                  << "Usage: mping-sender [configuration]" << std::endl
                  << std::endl
                  << options << std::endl;
        std::exit(EXIT_SUCCESS);
    }

    notify(vm);

    boost::log::core::get()->set_filter(boost::log::trivial::severity >=
                                        this->_log_level);
}

bool MPingSender::Configuration::get_help() const noexcept
{
    return this->_help;
}

boost::log::trivial::severity_level
    MPingSender::Configuration::get_log_level() const noexcept
{
    return this->_log_level;
}

std::string MPingSender::Configuration::get_bind_address() const noexcept
{
    return this->_bind_address;
}

uint16_t MPingSender::Configuration::get_bind_port() const noexcept
{
    return this->_bind_port;
}

std::string MPingSender::Configuration::get_address() const noexcept
{
    return this->_address;
}

uint16_t MPingSender::Configuration::get_port() const noexcept
{
    return this->_port;
}

uint8_t MPingSender::Configuration::get_ttl() const noexcept
{
    return this->_ttl;
}

std::string MPingSender::Configuration::get_interface_name() const noexcept
{
    return this->_interface_name;
}

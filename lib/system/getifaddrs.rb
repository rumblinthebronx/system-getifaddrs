if RUBY_ENGINE == "jruby"
  module System
    require 'netaddr'

    # Jruby version of the system-getifaddrs gem.  Gives info on all network
    # interfaces.
    #
    # @return [Hash] Network interfaces and their addresses/netmasks.
    def self.get_ifaddrs
      ifaddrs = {}
      interfaces = java.net.NetworkInterface.network_interfaces

      interfaces.each do |interface|
        name = interface.display_name.to_sym
        ifaddrs[name] = {}

        interface.interface_addresses.each do |interface_address|
          ip_address, index = interface_address.address.host_address.split('%')
          prefix = interface_address.network_prefix_length
          netmask = NetAddr::CIDR.create("#{ip_address}/#{prefix}").wildcard_mask

          if interface_address.address.is_a? Java::JavaNet::Inet4Address
            ifaddrs[name][:inet_addr_v4] = ip_address
            netmask = '255.0.0.0' if interface_address.address.is_loopback_address?
            ifaddrs[name][:netmask_v4] = netmask
          else
            ifaddrs[name][:inet_addr_v6] = "#{ip_address}%#{index}"
            ifaddrs[name][:netmask_v6] = NetAddr.shorten(netmask)
          end
        end
      end

      ifaddrs
    end
  end
else
  begin
    require File.join(File.dirname(__FILE__), 'rb_getifaddrs')
  rescue LoadError
    require File.join(File.dirname(__FILE__), '..', 'rb_getifaddrs')
  end
end
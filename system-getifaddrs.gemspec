# -*- encodign: utf-8 -*-
$:.push File.expand_path("../lib", __FILE__)
require "system/getifaddrs/version"

Gem::Specification.new do |s|
  s.name = "rumblinthebronx-system-getifaddrs"
  s.version = System::Getifaddrs::VERSION
  s.platform = Gem::Platform::RUBY
  s.platform = Gem::Platform.new(['universal', 'java']) if RUBY_PLATFORM == "java"
  s.authors = ["Sujin Philip", "Bruno Coimbra"]
  s.email = %q{sujin.phil@gmail.com}
  s.homepage = %q{http://github.com/rumblinthebronx/system-getifaddrs}
  s.summary = %q{This library provides the information about the inet interfaces. }
  s.description = %q{This lib is a wrapper for get_ifaddrs C routine. The original routine returns a linked list that contains avaliable inet interfaces. This lib walks on list and return an hash that contains the interface names and sub-hashes with respectives ip addresses and netmasks.}
  s.add_development_dependency(%q<rspec>, "~> 1.3")
  s.add_development_dependency(%q<rake-compiler>, [">= 0"])
  s.add_runtime_dependency(%q<netaddr>, "~> 1.5.0") if RUBY_PLATFORM == "java"

  unless RUBY_PLATFORM.include?("mingw") || RUBY_PLATFORM == "java"
    s.extensions    = `git ls-files -- ext/*`.split("\n").select{|f| f =~ /extconf/}
  end

  s.files         = `git ls-files`.split("\n")
  s.test_files    = `git ls-files -- {test,spec,features}/*`.split("\n")
  s.executables   = `git ls-files -- bin/*`.split("\n").map{ |f| File.basename(f) }
  s.require_paths = ["lib"]
end


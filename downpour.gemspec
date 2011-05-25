require File.expand_path('../lib/downpour/version', __FILE__)

Gem::Specification.new do |s|
  s.name = %q{downpour}
  s.version = Downpour::VERSION
  s.authors = ["Tejas Dinkar"]
  s.date = Time.now.utc.strftime("%Y-%m-%d")
  s.email = %q{tejas@gja.in}
  s.extensions = ["ext/downpour/extconf.rb"]
  s.extra_rdoc_files = [
    "README.rdoc"
  ]
  s.files = `git ls-files`.split("\n")
  s.homepage = %q{http://github.com/gja/downpour}
  s.rdoc_options = ["--charset=UTF-8"]
  s.require_paths = ["lib", "ext"]
  s.rubygems_version = %q{1.4.2}
  s.summary = %q{A simple, fast Mysql and Drizzle library for Ruby, binding to libdrizzle}
  s.test_files = `git ls-files spec examples`.split("\n")

  # tests
  s.add_development_dependency 'rake-compiler', "~> 0.7.1"
  s.add_development_dependency 'rspec'
end

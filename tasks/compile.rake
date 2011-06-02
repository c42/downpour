def gemspec
  @clean_gemspec ||= eval(File.read(File.expand_path('../../downpour.gemspec', __FILE__)))
end

begin
  require "rake/extensiontask"
  Rake::ExtensionTask.new("downpour", gemspec) do |ext|
    ext.lib_dir = File.join 'lib', 'downpour'

    CLEAN.include "#{ext.lib_dir}/*.#{RbConfig::CONFIG['DLEXT']}"
  end
  Rake::Task[:spec].prerequisites << :compile

  Rake::GemPackageTask.new(gemspec) do |pkg|
    pkg.need_zip = true
    pkg.need_tar = true
    pkg.package_dir = ENV['BUILD_ARTEFACTS'] || 'pkg'
  end
rescue LoadError
  puts "rake-compiler, or one of its dependencies, is not available. Install it with: sudo gem install rake-compirer"
end

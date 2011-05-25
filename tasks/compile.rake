require "rake/extensiontask"

def gemspec
  @clean_gemspec ||= eval(File.read(File.expand_path('../../downpour.gemspec', __FILE__)))
end

Rake::ExtensionTask.new("downpour", gemspec) do |ext|
  ext.lib_dir = File.join 'lib', 'downpour'

  # clean compiled extension
  CLEAN.include "#{ext.lib_dir}/*.#{RbConfig::CONFIG['DLEXT']}"
end
Rake::Task[:spec].prerequisites << :compile
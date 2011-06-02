# encoding: UTF-8
require 'rake'

# Load custom tasks
Dir['tasks/*.rake'].sort.each { |f| load f }

task :copy_gem_to_output => :gem do
  FileUtils.mv('pkg', ENV['BUILD_ARTEFACTS'] || 'output', :verbose => true)
end

desc 'To be used by the CI server'
task :ci do
  sh "sh start_local_drizzle_server_for_tests.sh"
  begin
    Rake::Task['spec'].invoke
  ensure
    sh "drizzle -u " + ENV["USER"] + " --shutdown"
  end
end

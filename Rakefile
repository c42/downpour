# encoding: UTF-8
require 'rake'

# Load custom tasks
Dir['tasks/*.rake'].sort.each { |f| load f }

desc 'To be used by the CI server'
task :ci => :clean do
  sh "sh start_local_drizzle_server_for_tests.sh"
  begin
    Rake::Task['spec'].invoke
    Rake::Task['gem'].invoke
  ensure
    sh "drizzle -u " + ENV["USER"] + " --shutdown"
  end
end

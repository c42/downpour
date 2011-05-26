# encoding: UTF-8
require 'rake'

# Load custom tasks
Dir['tasks/*.rake'].sort.each { |f| load f }

desc 'To be used by the CI server'
task :ci do
  begin
    sh "sh start_local_drizzle_server_for_tests.sh"
    Rake::Task['spec'].invoke
  ensure
    sh "drizzle --shutdown"
  end
end

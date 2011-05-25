require '../drizzle'

def create_connection(status)
  status.add_tcp_connection "localhost", ENV["USER"], "", "test"
end

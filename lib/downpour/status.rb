module Downpour
  class Status
    def add_query(connection, query)
      _add_query(connection, query)
    end

    def run!
      _run!
    end

    def run_all!
      _run_all!
    end
  end
end

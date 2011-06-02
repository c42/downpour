module Downpour
  class Status
    def add_query(connection, query)
      new_query = _add_query(connection, query)
      pending_queries << new_query
      new_query
    end

    def run!
      pending_queries.delete(_run!)
    end

    def run_all!
      _run_all!
      pending_queries.clear
    end

    def pending_queries
      if(@pending_queries == nil)
        @pending_queries = []
      end
      @pending_queries
    end
  end
end

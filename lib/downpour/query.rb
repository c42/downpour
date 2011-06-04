module Downpour
  class Query
    def result
      return @result if @result
      @result = create_results
    end
  end
end

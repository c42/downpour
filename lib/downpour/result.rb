module Downpour
  class Result
    def columns
      return @columns if @columns
      @columns = []
      while(column = next_column) do
        @columns<<column
      end
      @columns
    end
  end
end

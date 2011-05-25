require 'spec_helper'

describe Drizzle do
  before(:each) do
    @status = Drizzle.create
    @conn = @status.add_tcp_connection
  end

  it "should be able to count records" do
    results = @conn.query "select * from Test1"
    results.row_count.should == 3
  end
end

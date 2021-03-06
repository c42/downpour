describe "an unbuffered select query" do

  before(:each) do
    @status = Downpour.create
    @conn = create_connection(@status)
    @results = @conn.unbuffered_query "select * from Test1"
  end

  it "should be able to run a second query after all rows are read" do
    4.times { @results.next_row }
    @conn.query "select * from Test2"
  end

  it "should be able to run another query after closing the query" do
    @results.close!
    @conn.query "select * from Test2"
  end

  it "should be unbuffered" do
    @results.should_not be_buffered
  end

  it "should get column count" do
    @results.column_count.should == 1
  end
end

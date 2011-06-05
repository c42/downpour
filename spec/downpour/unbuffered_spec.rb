describe "an unbuffered select query" do

  before(:each) do
    @status = Downpour.create
    @conn = create_connection(@status)
    @results = @conn.unbuffered_query "select * from Test1"
  end

  it "should read all rows" do
    @results.next_row.should == ["foo"]
    @results.next_row.should == ["bar"]
    @results.next_row.should == ["baz"]
    @results.next_row.should be_nil
  end

  it "should be unbuffered" do
    @results.should_not be_buffered
  end

  it "should get column count" do
    @results.column_count.should == 1
  end
end

describe "a drizzle result" do

  before(:each) do
    @status = Downpour.create
    @conn = create_connection(@status)
    @results = @conn.query "select * from Test2"
  end

  it "should fetch all columns" do
    @results.columns.size.should == 2
  end

  it "should count columns" do
    @results.column_count.should == 2
  end

  it "should not buffer after reading columns" do
    @results.columns
    @results.should_not be_buffered
  end

  it "should be able to read after buffering" do
    @results.buffer!
    @results.columns.size.should == 2
  end
end

describe "a buffered query" do

  before(:each) do
    @status = Downpour.create
    @conn = create_connection(@status)
    @results = @conn.query "select * from Test1"
  end

  it "should count records" do
    @results.row_count.should == 3
  end

  it "should be able to run a second query" do
    @conn.query "select * from Test2"
  end

  it "should be buffered" do
    @results.should be_buffered
  end
end

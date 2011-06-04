describe "a drizzle result" do

  before(:each) do
    @status = Downpour.create
    @conn = create_connection(@status)
    @results = @conn.query "select id, name as newName from Test2"
  end

  it "should fetch all columns" do
    @results.columns.size.should == 2
  end

  it "should count columns" do
    @results.column_count.should == 2
  end

  it "should fetch column name" do
    @results.columns[0].name.should == "id"
    @results.columns[1].name.should == "newName"
  end

  it "should fetch the original name from table" do
    @results.columns[1].orig_name.should == "name"
  end

  it "should have table name set on column" do
    @results.columns[0].table.should == "Test2"
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

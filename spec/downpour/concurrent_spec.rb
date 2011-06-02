describe "a concurrent query" do

  before(:each) do
    @status = Downpour.create
    @conn = create_connection(@status)
    #@conn2 = create_connection(@status)
    @query1 = @status.add_query(@conn, "select * from Test1 where name like '%oo'");
    #@query2 = @status.add_query(@conn2, "select name from Test1 where name like '%ar'");
  end

  it "should buffer all queries" do
    @status.run_all!
    @query1.result.should be_buffered
    #@query2.result.should be_buffered
  end

  it "should run all queries" do
    @status.run_all!
    @query1.result.next_row.should == ["foo"]
    #@query2.result.next_row.should == ["bar"]
  end
end

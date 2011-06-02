describe "a concurrent query" do

  before(:each) do
    @status = Downpour.create
    @conn = create_connection(@status)
    @conn2 = create_connection(@status)
    @query1 = @status.add_query(@conn, "select * from Test1 where name like '%oo'");
    @query2 = @status.add_query(@conn2, "select name from Test1 where name like '%ar'");
  end

  context "when running all" do
    it "should buffer all queries" do
      @status.run_all!
      @query1.result.should be_buffered
      @query2.result.should be_buffered
    end

    it "should run all queries" do
      @status.run_all!
      @query1.result.next_row.should == ["foo"]
      @query2.result.next_row.should == ["bar"]
    end

    it "should clear all pending queries" do
      @status.pending_queries.size.should == 2
      @status.run_all!
      @status.pending_queries.should be_empty
    end
  end

  context "when executing one at a time" do
    it "should run all queries" do
      rows = []
      rows << @status.run!.result.next_row
      rows << @status.run!.result.next_row
      rows.should include(["foo"])
      rows.should include(["bar"])
    end

    it "should return nil after all queries are executed" do
      @status.run!
      @status.run!
      @status.run!.should be_nil
    end

    it "should release pending queries" do
      query = @status.run!
      @status.pending_queries.should_not include(query)
      @status.pending_queries.size.should == 1
    end
  end
end

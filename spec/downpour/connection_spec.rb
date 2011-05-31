describe Downpour do
  before(:each) do
    @status = Downpour.create
    @connection = create_connection(@status)
  end

  context "error handling" do

    it "should catch errors" do
      begin
        query = @connection.query "this is invalid sql"
        raise "should have failed here"
      rescue IOError
        @status.error.should == "You have an error in your SQL syntax; check the manual that corresponds to your Drizzle server version for the right syntax to use near 'this is invalid sql' at line 1"
      end
    end

    it "should only accept a string as query" do
      lambda {@connection.query 42}.should raise_error(TypeError)
    end
  end
  
  context "simple attributes" do
    it "should change the database" do
      @connection.db.should == "test"
      @connection.db = "blah"
      @connection.db.should == "blah"
    end

    it "should read simple attributes" do
      @connection.port.should == 4427
      @connection.user.should == ENV["USER"]
      @connection.password.should == ""
    end
  end

  it "should close the connection" do
    @connection.close!
    # TODO: Find a way to assert on this
  end
end

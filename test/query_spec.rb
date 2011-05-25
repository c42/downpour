require './spec_helper'

describe Drizzle do
  before(:each) do
    @status = Drizzle.create
    @connection = create_connection(@status)
  end

  context "error handling" do

    it "should be able to catch errors" do
      begin
        query = @connection.query "this is invalid sql"
        raise "should have failed here"
      rescue IOError
        @status.error.should == "You have an error in your SQL syntax; check the manual that corresponds to your Drizzle server version for the right syntax to use near 'this is invalid sql' at line 1"
      end
    end

    it "should only accept a string as query" do
      begin
        @connection.query 42
        raise "should have failed here"
      rescue TypeError
      end
    end

  end
end

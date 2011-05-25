require './spec_helper'

describe Drizzle do

  before(:each) do
    @status = Drizzle.create
    @conn = create_connection(@status)
  end

  context "a select query" do
    before(:each) do
      @results = @conn.query "select * from Test1"
    end

    it "should be able to count records" do
      @results.row_count.should == 3
    end

    it "should be able to count columns" do
      @results.column_count.should == 1
    end

    it "should be able to get a row" do
      @results.next_row.should == ["foo"]
      #@results.next_row.should == ["bar"]
      #@results.next_row.should == ["baz"]
      #@results.next_row.should == []
    end
  end

  after(:each) do
    error = @status.error
    if(error != "")
      puts error
    end
  end

end

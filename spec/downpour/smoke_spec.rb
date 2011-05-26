describe Downpour do

  before(:each) do
    @status = Downpour.create
    @conn = create_connection(@status)
  end

  context "select query" do
    before(:each) do
      @results = @conn.query "select * from Test1"
    end

    it "should count records" do
      @results.row_count.should == 3
      @results.should be_buffered
    end

    it "should buffer records" do
      @results.should_not be_buffered
      @results.buffer!
      @results.should be_buffered
    end

    it "should only buffer records once" do
      @results.buffer!.should be_true
      @results.buffer!.should be_false
    end

    it "should count columns" do
      @results.column_count.should == 1
    end

    it "should not buffer when reading a single row" do
      @results.next_row
      @results.should_not be_buffered
    end

    shared_examples_for "a read query" do
      it "should read all rows" do
        @results.next_row.should == ["foo"]
        @results.next_row.should == ["bar"]
        @results.next_row.should == ["baz"]
        @results.next_row.should be_nil
      end
    end

    context "without buffering" do
      #it_should_behave_like "a read query"
    end

    context "with buffering" do
      before(:each) { @results.buffer! }

      it_should_behave_like "a read query"
    end
  end

  after(:each) do
    #puts @status.error
  end
end

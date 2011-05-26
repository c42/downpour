describe Downpour do
  it "should have the version number set" do
    Downpour.drizzle_version.should eq("7")
  end

  it "should set the verbose level" do
    st = Downpour.create
    st.verbose = 5
    st.verbose.should == 5
    st.verbose_name.should == "CRAZY"
  end
end

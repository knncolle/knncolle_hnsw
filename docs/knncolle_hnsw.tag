<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile doxygen_version="1.12.0">
  <compound kind="file">
    <name>distances.hpp</name>
    <path>knncolle_hnsw/</path>
    <filename>distances_8hpp.html</filename>
    <class kind="struct">knncolle_hnsw::DistanceOptions</class>
    <class kind="class">knncolle_hnsw::ManhattanDistance</class>
    <class kind="class">knncolle_hnsw::SquaredEuclideanDistance</class>
    <namespace>knncolle_hnsw</namespace>
  </compound>
  <compound kind="file">
    <name>knncolle_hnsw.hpp</name>
    <path>knncolle_hnsw/</path>
    <filename>knncolle__hnsw_8hpp.html</filename>
    <includes id="distances_8hpp" name="distances.hpp" local="yes" import="no" module="no" objc="no">distances.hpp</includes>
    <class kind="struct">knncolle_hnsw::HnswOptions</class>
    <class kind="class">knncolle_hnsw::HnswSearcher</class>
    <class kind="class">knncolle_hnsw::HnswPrebuilt</class>
    <class kind="class">knncolle_hnsw::HnswBuilder</class>
    <namespace>knncolle_hnsw</namespace>
  </compound>
  <compound kind="struct">
    <name>knncolle_hnsw::DistanceOptions</name>
    <filename>structknncolle__hnsw_1_1DistanceOptions.html</filename>
    <templarg>typename HnswData_</templarg>
    <member kind="variable">
      <type>std::function&lt; hnswlib::SpaceInterface&lt; HnswData_ &gt; *(size_t)&gt;</type>
      <name>create</name>
      <anchorfile>structknncolle__hnsw_1_1DistanceOptions.html</anchorfile>
      <anchor>a8fd0302b1bd4723ccf2445819a2822ac</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::function&lt; HnswData_(HnswData_)&gt;</type>
      <name>normalize</name>
      <anchorfile>structknncolle__hnsw_1_1DistanceOptions.html</anchorfile>
      <anchor>ae84f48ee15b1c5b868dcba1c525dd2c0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>knncolle_hnsw::HnswBuilder</name>
    <filename>classknncolle__hnsw_1_1HnswBuilder.html</filename>
    <templarg>typename Index_</templarg>
    <templarg>typename Data_</templarg>
    <templarg>typename Distance_</templarg>
    <templarg>class Matrix_</templarg>
    <templarg>typename HnswData_</templarg>
    <base>knncolle::Builder&lt; typename Index_, typename Data_, typename Distance_, class Matrix_ &gt;</base>
    <member kind="typedef">
      <type>HnswOptions&lt; HnswData_ &gt;</type>
      <name>Options</name>
      <anchorfile>classknncolle__hnsw_1_1HnswBuilder.html</anchorfile>
      <anchor>a635d9595450dcb631735f47c35744d67</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>HnswBuilder</name>
      <anchorfile>classknncolle__hnsw_1_1HnswBuilder.html</anchorfile>
      <anchor>a663a7e751c364a0b57dab1521ecc442f</anchor>
      <arglist>(Options options)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>HnswBuilder</name>
      <anchorfile>classknncolle__hnsw_1_1HnswBuilder.html</anchorfile>
      <anchor>ae783debe4f4f5faf250417f3c231f635</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function">
      <type>Options &amp;</type>
      <name>get_options</name>
      <anchorfile>classknncolle__hnsw_1_1HnswBuilder.html</anchorfile>
      <anchor>a69f09f9e3a70555deacd75a68b22a79d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>knncolle::Prebuilt&lt; Index_, Data_, Distance_ &gt; *</type>
      <name>build_raw</name>
      <anchorfile>classknncolle__hnsw_1_1HnswBuilder.html</anchorfile>
      <anchor>ae96f13d033eff8af3996996b92e4f4b2</anchor>
      <arglist>(const Matrix_ &amp;data) const</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>knncolle_hnsw::HnswOptions</name>
    <filename>structknncolle__hnsw_1_1HnswOptions.html</filename>
    <templarg>typename HnswData_</templarg>
    <member kind="variable">
      <type>int</type>
      <name>num_links</name>
      <anchorfile>structknncolle__hnsw_1_1HnswOptions.html</anchorfile>
      <anchor>a32e962b43c4e61d3c21347d5176aea81</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>ef_construction</name>
      <anchorfile>structknncolle__hnsw_1_1HnswOptions.html</anchorfile>
      <anchor>a6cb3835caada27e57a982154d7e3e107</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>int</type>
      <name>ef_search</name>
      <anchorfile>structknncolle__hnsw_1_1HnswOptions.html</anchorfile>
      <anchor>a98e6598d587049c4cc239486755b80f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>DistanceOptions&lt; HnswData_ &gt;</type>
      <name>distance_options</name>
      <anchorfile>structknncolle__hnsw_1_1HnswOptions.html</anchorfile>
      <anchor>a6e9822c2ffbe095b1ca7a57164f59e1d</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>knncolle_hnsw::HnswPrebuilt</name>
    <filename>classknncolle__hnsw_1_1HnswPrebuilt.html</filename>
    <templarg>typename Index_</templarg>
    <templarg>typename Data_</templarg>
    <templarg>typename Distance_</templarg>
    <templarg>typename HnswData_</templarg>
    <base>knncolle::Prebuilt&lt; Index_, Data_, Distance_ &gt;</base>
    <member kind="function">
      <type>std::unique_ptr&lt; knncolle::Searcher&lt; Index_, Data_, Distance_ &gt; &gt;</type>
      <name>initialize</name>
      <anchorfile>classknncolle__hnsw_1_1HnswPrebuilt.html</anchorfile>
      <anchor>a7e68dbb7ed19a02b2abd3f51e85fc0f5</anchor>
      <arglist>() const</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>knncolle_hnsw::HnswSearcher</name>
    <filename>classknncolle__hnsw_1_1HnswSearcher.html</filename>
    <templarg>typename Index_</templarg>
    <templarg>typename Data_</templarg>
    <templarg>typename Distance_</templarg>
    <templarg>typename HnswData_</templarg>
    <base>knncolle::Searcher&lt; Index_, Data_, Distance_ &gt;</base>
  </compound>
  <compound kind="class">
    <name>knncolle_hnsw::ManhattanDistance</name>
    <filename>classknncolle__hnsw_1_1ManhattanDistance.html</filename>
    <templarg>typename HnswData_</templarg>
    <member kind="function">
      <type></type>
      <name>ManhattanDistance</name>
      <anchorfile>classknncolle__hnsw_1_1ManhattanDistance.html</anchorfile>
      <anchor>ada1953be51051bdb1629d58d46c10bf2</anchor>
      <arglist>(size_t dim)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>knncolle_hnsw::SquaredEuclideanDistance</name>
    <filename>classknncolle__hnsw_1_1SquaredEuclideanDistance.html</filename>
    <templarg>typename HnswData_</templarg>
    <member kind="function">
      <type></type>
      <name>SquaredEuclideanDistance</name>
      <anchorfile>classknncolle__hnsw_1_1SquaredEuclideanDistance.html</anchorfile>
      <anchor>a2e24b25ce5f76fcdfa015bc645a9b95b</anchor>
      <arglist>(size_t dim)</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>knncolle_hnsw</name>
    <filename>namespaceknncolle__hnsw.html</filename>
    <class kind="struct">knncolle_hnsw::DistanceOptions</class>
    <class kind="class">knncolle_hnsw::HnswBuilder</class>
    <class kind="struct">knncolle_hnsw::HnswOptions</class>
    <class kind="class">knncolle_hnsw::HnswPrebuilt</class>
    <class kind="class">knncolle_hnsw::HnswSearcher</class>
    <class kind="class">knncolle_hnsw::ManhattanDistance</class>
    <class kind="class">knncolle_hnsw::SquaredEuclideanDistance</class>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>knncolle bindings for HNSW</title>
    <filename>index.html</filename>
    <docanchor file="index.html">md__2github_2workspace_2README</docanchor>
  </compound>
</tagfile>

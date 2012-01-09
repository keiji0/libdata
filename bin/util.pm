package util;

sub ifndef_name($) {
  my $file=$_[0];
  $file =~ s/[\/\.]/_/g;
  return uc $file;
}

1;

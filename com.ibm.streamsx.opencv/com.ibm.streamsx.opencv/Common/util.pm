# Licensed Materials - Property of IBM
# Copyright IBM Corp. 2012
# US Government Users Restricted Rights - Use, duplication or
# disclosure restricted by GSA ADP Schedule Contract with
# IBM Corp.

package util;

use strict ;

sub true {return "true";}
sub false {return "false";}

require Exporter;
our @ISA = qw(Exporter);
our @EXPORT = qw(fail qt get_param xfloatschema cpuspeed input_attribute_map output_attribute_map attr_names globlist) ;

my $op = "(" ;
my $cp = ")" ;

my $osb = "[[]" ;
my $csb = "[]]" ;

my $nsb = "^]" ;

my $dummy_count = 0 ;

sub fail($)
{
  my ($msg) = @_ ;

  die "\n\n *** $msg ***\n\n" ;
}

sub qt($)
{
  return "\"@_\"" ;
}

sub var_cleanup($)
{
    my ($v_org) = @_ ;
    
    my $v = $v_org ;
    
    while(1)
    {
        my $before_v = $v ;

        $v =~ s/static_cast<[^>]+>\($op[^$cp]+$cp\)/\1/ ;
        if ($v eq $before_v)
        {
            $v =~ s/^std::string\((.*)\)$/\1/ ;
            if ($v eq $before_v)
            {
                last ;
            }
        }
    }
    
    my $var = "error" ;
    eval("no warnings 'all' ; \$var = $v ;") ;
    if ($@)
    {
        my $err = $@ ;
        chomp $err ;
        for my $line (split/\n/, $err)
        {
            print STDERR "Error: $line\n";
        }
        return $v_org ;
    }
    
    return $var ;
}

sub get_param($$$)
{
  my ($model, $name, $dft) = @_ ;

  if ($model->getParameterByName($name))
  {
    my @results ;

    for(my $i=0; $i<$model->getParameterByName($name)->getNumberOfValues( ); $i++)
    {
      my $v = $model->getParameterByName($name)->getValueAt($i)->getSPLExpression() ;

      if ($v =~ m/^_cmd_/)
      {
        push(@results, $v) ;
      }
      else
      {
        push(@results, var_cleanup($v)) ;
      }
    }

    if (wantarray)
    {
      return @results ;
    }
    else
    {
      return $results[0] ;
    }
  }
  else
  {
    print STDERR "Parameter '$name' undefined. Using default\n" if $model->getContext()->isVerboseModeOn( ) ;

    if (wantarray)
    {
      if ($dft)
      {
        fail("The result for parameter $name is expected to be an array but the default is not an array") if ref($dft) ne "ARRAY" ;
        
        return @{$dft} ;
      }
      else
      {
        return () ;
      }
    }
    else
    {
      fail("The result for parameter $name is expected to be a scalar but the default is not an scalar") if $dft && ref(\$dft) ne "SCALAR" ;
      
      return $dft ;
    }
  }
}

sub xfloatschema($$)
{
  my ($numfloats, $comma) = @_ ;
  my @res = () ;

  for(my $f=1; $f<=$numfloats; $f++)
  {
    push(@res, sprintf("f%d:Float", $f)) ;
  }

  my $res = join(",", @res) ;

  if ($comma && $res)
  {
    return ",$res" ;
  }
  else
  {
    return $res ;
  }
}

sub cpuspeed()
{
  my $procfile = "/proc/cpuinfo" ;
  open(FD, "< $procfile") || die "Unable to open $procfile" ;

  while(<FD>)
  {
    chomp $_ ;
  
    if ($_ =~ m/^cpu MHz[\t ]+: ([0-9.]+)$/)
    {
      return $1 * 1e6 ;
    }
  }

  return undef ;
}

sub input_attribute_map($)
{
  my ($model) = @_ ;

  my $num_ports = $model->getNumberOfInputPorts() ;

  my %attr_map ;

  for (my $pid=0 ; $pid < $num_ports; $pid++)
  {
    foreach my $attribute (@{$model->getInputPortAt($pid)->getAttributes( )})
    {
      if (defined $attr_map{$attribute->getName( )})
      {
        fail("Attribute '" . $attribute->getName( ) . "' defined more than once") ;
      }
      
      $attr_map{$attribute->getName( )} = $pid ;
    }
  }

  return \%attr_map ;
}

sub output_attribute_map($)
{
  my ($model) = @_ ;

  my $num_ports = $model->getNumberOfOutputPorts() ;

  my %attr_map ;

  for (my $pid=0 ; $pid < $num_ports; $pid++)
  {
    foreach my $attribute (@{$model->getOutputPortAt($pid)->getAttributes( )})
    {
      if (defined $attr_map{$attribute->getName( )})
      {
        fail("Attribute '" . $attribute->getName( ) . "' defined more than once") ;
      }
      
      $attr_map{$attribute->getName( )} = $pid ;
    }
  }

  return \%attr_map ;
}

sub attr_names
{
  my $coll_ref = shift(@_) ;
  my @conds = @_ ;
  my @res ;

  foreach my $c (@{$coll_ref})
  {
    foreach my $attr (@{$c->getAttributes( )})
    {
      my $skip_it = 0 ;
      my $name = $attr->getName( ) ;

      foreach my $pttrn (@conds)
      {
        if ($name =~ m/$pttrn/)
        {
          $skip_it = 1 ;
          last ;
        }
      }

      push(@res, $name) unless $skip_it ;
    }
  }

  return @res ;
}

sub globlist($)
{
  my ($v) = @_ ;

  my $res ;

  for my $d (glob($v))
  {
    $res = "$res \"$d\"" ;
  }

  return $res ;
}

1

# Local Variables: ***
# mode:perl ***
# perl-indent-level: 2 ***
# End: ***

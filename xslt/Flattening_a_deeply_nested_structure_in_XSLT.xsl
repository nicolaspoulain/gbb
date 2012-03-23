<xsl:comment>From http://stackoverflow.com/questions/1912802/flattening-a-deeply-nested-structure-in-xslt</xsl:comment> 

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

  <xsl:template match="/">
    <html>
      <body>
        <xsl:apply-templates />
      </body>
    </html>
  </xsl:template>

  <xsl:template match="title">
    <xsl:choose>
      <xsl:when test="count(ancestor::section) = 1">
        <h1><xsl:value-of select="." /></h1>
      </xsl:when>
      <xsl:when test="count(ancestor::section) = 2">
        <h2><xsl:value-of select="." /></h2>
      </xsl:when>
      <xsl:otherwise>
        <h3><xsl:value-of select="." /></h3>
      </xsl:otherwise>
    </xsl:choose>
  </xsl:template>

  <xsl:template match="para">
    <p><xsl:value-of select="." /></p>
  </xsl:template>

</xsl:stylesheet>

